#include "AllPieces.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "../lois/Uniform_Discreet_Law.hpp"
#include "piece.hpp"
#include "quick_imgui/quick_imgui.hpp"

std::vector<std::pair<std::string, std::string>> piecePaths;
std::map<std::string, GLuint>                    m_textures;

GLuint LoadTexture(const char* path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        std::cerr << "Failed to open image: " << path << std::endl;
        return 0;
    }

    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);

    if (header[0] != 'B' || header[1] != 'M')
    {
        std::cerr << "Not a BMP file: " << path << std::endl;
        return 0;
    }

    int dataPos   = *(int*)&(header[0x0A]);
    int width     = *(int*)&(header[0x12]);
    int height    = *(int*)&(header[0x16]);
    int imageSize = *(int*)&(header[0x22]);

    // Calculer la taille réelle de l'image avec 32 bits par pixel
    int correctedImageSize = width * height * 4;

    if (imageSize == 0)
        imageSize = correctedImageSize;
    if (dataPos == 0)
        dataPos = 54;

    std::vector<unsigned char> data(imageSize);
    file.seekg(dataPos, std::ios::beg);
    if (file.read(reinterpret_cast<char*>(data.data()), imageSize).gcount() != imageSize)
    {
        std::cerr << "Failed to read the entire image data." << std::endl;
        return 0;
    }
    file.close();

    // Retourner l'image verticalement
    int                        rowSize = width * 4; // 4 octets par pixel (RGBA)
    std::vector<unsigned char> tempRow(rowSize);

    for (int y = 0; y < height / 2; y++)
    {
        unsigned char* row1 = &data[y * rowSize];
        unsigned char* row2 = &data[(height - 1 - y) * rowSize];

        // Échanger row1 et row2
        std::memcpy(tempRow.data(), row1, rowSize);
        std::memcpy(row1, row2, rowSize);
        std::memcpy(row2, tempRow.data(), rowSize);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Charger les données de l'image dans la texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    // Paramètres de la texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Vérification des erreurs OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << error << std::endl;
    }

    return textureID;
}

void AllPieces::InitializeAllPieces()
{
    // Efface toutes les pièces existantes
    m_black_pieces.clear();
    m_white_pieces.clear();
    for (int j{0}; j < 8; j++)
    {
        this->m_black_pieces.push_back(Piece(false, {1, j}, PieceType::Pawn));
    }
    for (int i{0}; i < 2; i++)
    {
        this->m_black_pieces.push_back(Piece(false, {0, i * 7}, PieceType::Rook));
        int pos_knight = i == 0 ? 1 : 6, pos_bishop = i == 0 ? 2 : 5;
        this->m_black_pieces.push_back(Piece(false, {0, pos_knight}, PieceType::Knight));
        this->m_black_pieces.push_back(Piece(false, {0, pos_bishop}, PieceType::Bishop));
    }
    this->m_black_pieces.push_back(Piece(false, {0, 4}, PieceType::Queen));
    this->m_black_pieces.push_back(Piece(false, {0, 3}, PieceType::King));

    for (int j{0}; j < 8; j++)
    {
        this->m_white_pieces.push_back(Piece(true, {6, j}, PieceType::Pawn));
    }
    for (int i{0}; i < 2; i++)
    {
        this->m_white_pieces.push_back(Piece(true, {7, i * 7}, PieceType::Rook));
        int pos_knight = i == 0 ? 1 : 6, pos_bishop = i == 0 ? 2 : 5;
        this->m_white_pieces.push_back(Piece(true, {7, pos_knight}, PieceType::Knight));
        this->m_white_pieces.push_back(Piece(true, {7, pos_bishop}, PieceType::Bishop));
    }
    this->m_white_pieces.push_back(Piece(true, {7, 4}, PieceType::Queen));
    this->m_white_pieces.push_back(Piece(true, {7, 3}, PieceType::King));

    // load les textures de pieces 2D
    piecePaths = {
        {"B_Pawn", "../../images/2D/Blacks/black-pawn.bmp"},
        {"B_Rook", "../../images/2D/Blacks/black-rook.bmp"},
        {"B_Knight", "../../images/2D/Blacks/black-knight.bmp"},
        {"B_Bishop", "../../images/2D/Blacks/black-bishop.bmp"},
        {"B_Queen", "../../images/2D/Blacks/black-queen.bmp"},
        {"B_King", "../../images/2D/Blacks/black-king.bmp"},
        {"W_Pawn", "../../images/2D/Whites/white-pawn.bmp"},
        {"W_Rook", "../../images/2D/Whites/white-rook.bmp"},
        {"W_Knight", "../../images/2D/Whites/white-knight.bmp"},
        {"W_Bishop", "../../images/2D/Whites/white-bishop.bmp"},
        {"W_Queen", "../../images/2D/Whites/white-queen.bmp"},
        {"W_King", "../../images/2D/Whites/white-king.bmp"}
    };

    for (const auto& piecePath : piecePaths)
    {
        m_textures[piecePath.first] = LoadTexture(piecePath.second.c_str());
    }
}

GLuint AllPieces::PiecesAppear(int x, int y) // GLuint ou //std::string
{
    // for (const auto& piece : m_black_pieces)
    // {
    //     if (piece.getCoords() == std::make_pair(x, y))
    //     {
    //         switch (piece.getType())
    //         {
    //         case PieceType::Pawn: return "B_Pawn";
    //         case PieceType::Rook: return "B_Rook";
    //         case PieceType::Knight: return "B_Knight";
    //         case PieceType::Bishop: return "B_Bishop";
    //         case PieceType::Queen: return "B_Queen";
    //         case PieceType::King: return "B_King";
    //         default:
    //             return "";
    //             break;
    //         }
    //     }
    // }

    // for (const auto& piece : m_white_pieces)
    // {
    //     if (piece.getCoords() == std::make_pair(x, y))
    //     {
    //         switch (piece.getType())
    //         {
    //         case PieceType::Pawn: return "W_Pawn";
    //         case PieceType::Rook: return "W_Rook";
    //         case PieceType::Knight: return "W_Knight";
    //         case PieceType::Bishop: return "W_Bishop";
    //         case PieceType::Queen: return "W_Queen";
    //         case PieceType::King: return "W_King";
    //         default:
    //             return "";
    //             break;
    //         }
    //     }
    // }
    // return "";
    for (const auto& piece : m_black_pieces)
    {
        if (piece.getCoords() == std::make_pair(x, y))
        {
            std::string textureKey = "B_";
            switch (piece.getType())
            {
            case PieceType::Pawn: textureKey += "Pawn"; break;
            case PieceType::Rook: textureKey += "Rook"; break;
            case PieceType::Knight: textureKey += "Knight"; break;
            case PieceType::Bishop: textureKey += "Bishop"; break;
            case PieceType::Queen: textureKey += "Queen"; break;
            case PieceType::King: textureKey += "King"; break;
            default:
                textureKey += "";
                break;
            }
            return m_textures[textureKey];
        }
    }

    for (const auto& piece : m_white_pieces)
    {
        if (piece.getCoords() == std::make_pair(x, y))
        {
            std::string textureKey = "W_";
            switch (piece.getType())
            {
            case PieceType::Pawn: textureKey += "Pawn"; break;
            case PieceType::Rook: textureKey += "Rook"; break;
            case PieceType::Knight: textureKey += "Knight"; break;
            case PieceType::Bishop: textureKey += "Bishop"; break;
            case PieceType::Queen: textureKey += "Queen"; break;
            case PieceType::King: textureKey += "King"; break;
            default:
                textureKey += "";
                break;
            }
            return m_textures[textureKey];
        }
    }
    return 0;
}

Piece* AllPieces::GetPieceAt(std::pair<int, int> coords)
{
    for (auto& piece : m_black_pieces)
    {
        if (piece.getCoords() == coords)
        {
            return &piece;
        }
    }
    for (auto& piece : m_white_pieces)
    {
        if (piece.getCoords() == coords)
        {
            return &piece;
        }
    }
    return nullptr;
}

void AllPieces::RemovePieceAt(std::pair<int, int> coords)
{
    // auto removePiece = [&](std::vector<Piece>& pieces) {
    //     auto it = std::remove_if(pieces.begin(), pieces.end(), [&](const Piece& piece) {
    //         return piece.getCoords() == coords;
    //     });
    //     if (it != pieces.end())
    //     {
    //         pieces.erase(it, pieces.end());
    //     }
    // };

    // removePiece(m_black_pieces);
    // removePiece(m_white_pieces);
    for (size_t i = 0; i < m_black_pieces.size(); ++i) {
        if (m_black_pieces[i].getCoords() == coords) {
            // Marquer la pièce comme morte
            m_black_pieces[i].setStatus(false);  // Marquer la pièce comme morte
            m_black_pieces[i].move({-1, -1});    // Déplacer la pièce hors du plateau
        }
    }
    
    for (size_t i = 0; i < m_white_pieces.size(); ++i) {
        if (m_white_pieces[i].getCoords() == coords) {
            // Marquer la pièce comme morte
            m_white_pieces[i].setStatus(false);  // Marquer la pièce comme morte
            m_white_pieces[i].move({-1, -1});    // Déplacer la pièce hors du plateau
        }
    }
}
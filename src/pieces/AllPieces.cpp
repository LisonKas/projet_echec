#include "AllPieces.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "piece.hpp"

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
        m_black_pieces.push_back(Piece(false, {1, j}, PieceType::Pawn));
    }
    for (int i{0}; i < 2; i++)
    {
        m_black_pieces.push_back(Piece(false, {0, i * 7}, PieceType::Rook));
        int pos_knight = i == 0 ? 1 : 6, pos_bishop = i == 0 ? 2 : 5;
        m_black_pieces.push_back(Piece(false, {0, pos_knight}, PieceType::Knight));
        m_black_pieces.push_back(Piece(false, {0, pos_bishop}, PieceType::Bishop));
    }
    m_black_pieces.push_back(Piece(false, {0, 4}, PieceType::Queen));
    m_black_pieces.push_back(Piece(false, {0, 3}, PieceType::King));

    for (int j{0}; j < 8; j++)
    {
        m_white_pieces.push_back(Piece(true, {6, j}, PieceType::Pawn));
    }
    for (int i{0}; i < 2; i++)
    {
        m_white_pieces.push_back(Piece(true, {7, i * 7}, PieceType::Rook));
        int pos_knight = i == 0 ? 1 : 6, pos_bishop = i == 0 ? 2 : 5;
        m_white_pieces.push_back(Piece(true, {7, pos_knight}, PieceType::Knight));
        m_white_pieces.push_back(Piece(true, {7, pos_bishop}, PieceType::Bishop));
    }
    m_white_pieces.push_back(Piece(true, {7, 4}, PieceType::Queen));
    m_white_pieces.push_back(Piece(true, {7, 3}, PieceType::King));

    // Charger les textures des pièces 2D
    piecePaths.push_back(std::make_pair("B_Pawn", "../../images/2D/Blacks/black-pawn.bmp"));
    piecePaths.push_back(std::make_pair("B_Rook", "../../images/2D/Blacks/black-rook.bmp"));
    piecePaths.push_back(std::make_pair("B_Knight", "../../images/2D/Blacks/black-knight.bmp"));
    piecePaths.push_back(std::make_pair("B_Bishop", "../../images/2D/Blacks/black-bishop.bmp"));
    piecePaths.push_back(std::make_pair("B_Queen", "../../images/2D/Blacks/black-queen.bmp"));
    piecePaths.push_back(std::make_pair("B_King", "../../images/2D/Blacks/black-king.bmp"));
    piecePaths.push_back(std::make_pair("W_Pawn", "../../images/2D/Whites/white-pawn.bmp"));
    piecePaths.push_back(std::make_pair("W_Rook", "../../images/2D/Whites/white-rook.bmp"));
    piecePaths.push_back(std::make_pair("W_Knight", "../../images/2D/Whites/white-knight.bmp"));
    piecePaths.push_back(std::make_pair("W_Bishop", "../../images/2D/Whites/white-bishop.bmp"));
    piecePaths.push_back(std::make_pair("W_Queen", "../../images/2D/Whites/white-queen.bmp"));
    piecePaths.push_back(std::make_pair("W_King", "../../images/2D/Whites/white-king.bmp"));

    for (size_t i = 0; i < piecePaths.size(); ++i)
    {
        m_textures[piecePaths[i].first] = LoadTexture(piecePaths[i].second.c_str());
    }
}

GLuint AllPieces::PiecesAppear(int x, int y)
{
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
            default: break;
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
            default: break;
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
    for (size_t i = 0; i < m_black_pieces.size(); ++i)
    {
        if (m_black_pieces[i].getCoords() == coords)
        {
            // Marquer la pièce comme morte
            m_black_pieces[i].setStatus(false); // Marquer la pièce comme morte
            m_black_pieces[i].move({-1, -1});   // Déplacer la pièce hors du plateau
        }
    }

    for (size_t i = 0; i < m_white_pieces.size(); ++i)
    {
        if (m_white_pieces[i].getCoords() == coords)
        {
            // Marquer la pièce comme morte
            m_white_pieces[i].setStatus(false); // Marquer la pièce comme morte
            m_white_pieces[i].move({-1, -1});   // Déplacer la pièce hors du plateau
        }
    }
}

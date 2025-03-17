#include "AllPieces.hpp"
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include "piece.hpp"
#include "../lois/Uniform_Discreet_Law.hpp"
#include "quick_imgui/quick_imgui.hpp"

std::vector<std::pair<std::string, std::string>> piecePaths;
std::map<std::string, GLuint> m_textures;

GLuint LoadTexture(const char* path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open image: " << path << std::endl;
        return 0;
    }

    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);

    if (header[0] != 'B' || header[1] != 'M') {
        std::cerr << "Not a BMP file: " << path << std::endl;
        return 0;
    }

    int dataPos = *(int*)&(header[0x0A]);
    int width = *(int*)&(header[0x12]);
    int height = *(int*)&(header[0x16]);
    int imageSize = *(int*)&(header[0x22]);

    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0) dataPos = 54;

    std::vector<unsigned char> data(imageSize);
    file.seekg(dataPos, std::ios::beg);
    file.read(reinterpret_cast<char*>(data.data()), imageSize);
    file.close();

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

void AllPieces::InitializeAllPieces()
{
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

    //load les textures de pieces 2D
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

    for (const auto& piecePath : piecePaths) {
        m_textures[piecePath.first] = LoadTexture(piecePath.second.c_str());
    }
}

GLuint AllPieces::PiecesAppear(int x, int y) //GLuint ou //std::string
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
    for (const auto& piece : m_black_pieces) {
        if (piece.getCoords() == std::make_pair(x, y)) {
            std::string textureKey = "B_";
            switch (piece.getType())
            {
                case PieceType::Pawn: textureKey += "Pawn";
                case PieceType::Rook: textureKey += "Rook";
                case PieceType::Knight: textureKey += "Knight";
                case PieceType::Bishop: textureKey += "Bishop";
                case PieceType::Queen: textureKey += "Queen";
                case PieceType::King: textureKey += "King";
                default:
                    textureKey += "";
                    break;
            }
            return m_textures[textureKey];
        }
    }

    for (const auto& piece : m_white_pieces) {
        if (piece.getCoords() == std::make_pair(x, y)) {
            std::string textureKey = "W_";
            switch (piece.getType())
            {
                case PieceType::Pawn: textureKey += "Pawn";
                case PieceType::Rook: textureKey += "Rook";
                case PieceType::Knight: textureKey += "Knight";
                case PieceType::Bishop: textureKey += "Bishop";
                case PieceType::Queen: textureKey += "Queen";
                case PieceType::King: textureKey += "King";
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

#include "AllPieces.hpp"
#include <iostream>
#include "piece.hpp"
#include "quick_imgui/quick_imgui.hpp"

void AllPieces::InitializeAllPieces(){
    for(int j {0}; j<8; j++){
        this->m_black_pieces.push_back(Piece(false, {1, j+1}, PieceType::Pawn));
    }
    for(int i {0}; i<2; i++){
        this->m_black_pieces.push_back(Piece(false, {0, i*7}, PieceType::Rook));
        int pos_knight = i==0 ? 1 : 6, pos_bishop = i==0 ? 2 : 5;
        this->m_black_pieces.push_back(Piece(false, {0, pos_knight}, PieceType::Knight));
        this->m_black_pieces.push_back(Piece(false, {0, pos_bishop}, PieceType::Bishop));
    }
    this->m_black_pieces.push_back(Piece(false, {0, 3}, PieceType::Queen));
    this->m_black_pieces.push_back(Piece(false, {0, 4}, PieceType::King));


    for(int j {0}; j<8; j++){
        this->m_white_pieces.push_back(Piece(true, {6, j+1}, PieceType::Pawn));
    }
    for(int i {0}; i<2; i++){
        this->m_white_pieces.push_back(Piece(true, {7, i*7}, PieceType::Rook));
        int pos_knight = i==0 ? 1 : 6, pos_bishop = i==0 ? 2 : 5;
        this->m_white_pieces.push_back(Piece(true, {7, pos_knight}, PieceType::Knight));
        this->m_white_pieces.push_back(Piece(true, {7, pos_bishop}, PieceType::Bishop));
    }
    this->m_white_pieces.push_back(Piece(true, {7, 3}, PieceType::Queen));
    this->m_white_pieces.push_back(Piece(true, {7, 4}, PieceType::King));

    std::cout << "All pieces are created." << std::endl;
}

void AllPieces::PiecesAppear(){
    for (int i {0}; i<m_black_pieces.size(); i++) {
        std::pair<int, int> coords = m_black_pieces[i].getCoords();
        
        switch(m_black_pieces[i].getType()){
            case PieceType::Pawn:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 + 65 , coords.first * 100 + 70)); 
                ImGui::Text("B_Pawn");
                break;

            case PieceType::Rook:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 - 20, coords.first * 100 + 70)); 
                ImGui::Text("B_Rook");
                break;

            case PieceType::Knight:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 - 20, coords.first * 100 + 70)); 
                ImGui::Text("B_Knight");
                break;

            case PieceType::Bishop:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 - 20, coords.first * 100 + 70)); 
                ImGui::Text("B_Bishop");
                break;

            case PieceType::Queen:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 - 20, coords.first * 100 + 70)); 
                ImGui::Text("B_Queen");
                break;

            case PieceType::King:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 - 20, coords.first * 100 + 70)); 
                ImGui::Text("B_King");
                break;

            default:
                break;
        }
    }

    for (int i {0}; i < m_white_pieces.size(); i++) {
        std::pair<int, int> coords = m_white_pieces[i].getCoords();
    
        switch (m_white_pieces[i].getType()) {
            case PieceType::Pawn:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 + 65, coords.first * 100 + 100)); 
                ImGui::Text("W_Pawn");
                break;

            case PieceType::Rook:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 - 20, coords.first * 100 + 100)); 
                ImGui::Text("W_Rook");
                break;

            case PieceType::Knight:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 - 20, coords.first * 100 + 100)); 
                ImGui::Text("W_Knight");
                break;

            case PieceType::Bishop:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 - 20, coords.first * 100 + 100)); 
                ImGui::Text("W_Bishop");
                break;

            case PieceType::Queen:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 - 20, coords.first * 100 + 100)); 
                ImGui::Text("W_Queen");
                break;

            case PieceType::King:
                ImGui::SetCursorPos(ImVec2((8 - coords.second) * 100 - 20, coords.first * 100 + 100)); 
                ImGui::Text("W_King");
                break;

            default:
                break;
        }
    }
}
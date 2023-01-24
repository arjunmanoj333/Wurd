#include "StudentTextEditor.h"
#include "Undo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

TextEditor* createTextEditor(Undo* un){
    return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo) : TextEditor(undo){
    m_row = 0;
    m_col = 0;
    m_text.push_front("");
    m_it = m_text.begin();
}

StudentTextEditor::~StudentTextEditor(){
}

bool StudentTextEditor::load(std::string file){
    ifstream infile(file);
    //if file not found return false
    if(!infile){
        return false;
    }
    //before reading in delete what's currently stored, O(M)
    reset();

    //read in file into list, O(N)
    string s;
    while (getline(infile, s)){
        if(s.size() >= 2 && '/r' + "" == s.substr(s.size() - 2, 1))
            s.erase(s.size() - 2, 1);
        m_text.push_back(s);
        cout << s << endl;
    }

    //after reading everything in from file reset current position to 0,0
    m_row = 0;
    m_col = 0;
    m_it = m_text.begin();
    return true;
}

bool StudentTextEditor::save(std::string file){
    //if output file cannot be opened return false
    ofstream outfile(file);
    if(!outfile){
        return false;
    }
    for (string const& i : m_text){
        outfile << i << endl;
    }
    return true;
}

void StudentTextEditor::reset(){
    m_text.clear();
    getUndo()->clear();
}

void StudentTextEditor::move(Dir dir){
    switch(dir){
        case Dir::UP:
            if(m_row > 0){
                m_row--;
                m_it--;
                if(m_col > m_it->size())
                    m_col = m_it->size();
            }
            else if(m_row == 0)
                m_col = 0;
            break;
        case Dir::DOWN:
            if(m_row < m_text.size() - 1){
                m_row++;
                m_it++;
                if (m_col > m_it->size())
                    m_col = m_it->size();
            }
            else if (m_row == m_text.size()-1)
                m_col = m_it->size();
            break;
        case Dir::LEFT:
            if(m_col > 0)
                m_col--;
            else if(m_col == 0){
                if (m_row != 0) {
                    m_row--;
                    m_it--;
                    m_col = m_it->size();
                }
            }
            break;
        case Dir::RIGHT:
            if(m_col < m_it->size())
                m_col++;
            else if(m_col == m_it->size()){
                if (m_row != m_text.size() - 1) {
                    m_row++;
                    m_it++;
                    m_col = 0;
                }
            }
            break;
        case Dir::HOME:
            m_col = 0;
            break;
        case Dir::END:
            m_col = m_it->size();
            break;
        default:
            break;
    }
}

void StudentTextEditor::del(){
    // TODO
}

void StudentTextEditor::backspace(){
    // TODO
}

void StudentTextEditor::insert(char ch){
    switch (ch){
    case '\t':
        for (int i = 0; i < 4;i++){
            m_it->insert(m_col, 1, ' ');

        }
        m_col += 4;
        getUndo()->submit(Undo::Action::INSERT, m_row, m_col + 1, ch);
        break;
    default:
        m_it->insert(m_col, 1, ch);
        m_col++;
        getUndo()->submit(Undo::Action::INSERT, m_row, m_col + 1, ch);
        break;
    }
}

void StudentTextEditor::enter(){
    //save text after where the cursor is
    string textToCopy;
    if(m_col == m_it->size())
        textToCopy = "";
    else
        textToCopy = m_it->substr(m_col, m_it->size() - m_col - 1);
    //delete text after where the cursor is in current line
    *m_it = (*m_it).substr(0, m_col);
    //create new line with saved text after current line.
    m_it++;
    m_row++;
    m_it = m_text.insert(m_it, textToCopy);
    m_col = 0;
    getUndo()->submit(Undo::Action::SPLIT, m_row, m_col);
}

void StudentTextEditor::getPos(int& row, int& col) const {
    col = m_col;
    row = m_row;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
    if(m_text.size() < startRow || startRow < 0 || numRows < 0)
        return -1;
    if(startRow == m_text.size())
        return 0;
    list<string>::const_iterator it = m_it;
    lines.clear();
    for (int x = 0; x < abs(startRow - m_row); x++){
        if(m_row < startRow)
            it++;
        else if(m_row > startRow)
            it--;
    }
    int x = 0;
    for (; it != m_text.end() && x < numRows; it++, x++)
        lines.push_back(*it);
    return lines.size();
}

void StudentTextEditor::undo(){
    // TODO
}


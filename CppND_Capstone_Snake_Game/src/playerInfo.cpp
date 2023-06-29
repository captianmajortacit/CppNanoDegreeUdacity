#ifndef PLAYERINFO_H
  #include "playerInfo.h"
#endif

#include<iostream>
#include<fstream>
#include<algorithm>

// Constructor
// cPlayerInfo()
// Description: initializes the data members
//              and retreives information from the ScoreSheet.txt
//              to fill the data member_table
// return type: none
// arguments  : none
cPlayerInfo::cPlayerInfo():
_currPlayer({"DEFAULT",0,0}){
    std::cout<<"Enter Player Name:";
    std::getline(std::cin,_currPlayer._name);
    ReadPlayerScoreHistory(); //reading all scores available in the ScoreSheet.txt and storing it to _table
}

// void ReadPlayerScoreHistory()
// Description: Checks if the file ScoreSheet.txt exists, 
//              then extract the information to the vector _table
//              else set the error message no records founds
// return type: void
// arguments  : none
void cPlayerInfo::ReadPlayerScoreHistory(){
    std::ifstream loc_inFile("ScoreSheet.txt");
    if(loc_inFile.is_open())
    {
        std::string loc_name;
        int loc_score, loc_DifficultyLevel;
        while(loc_inFile>>loc_name>>loc_score>>loc_DifficultyLevel){
            _table.push_back({loc_name, loc_score, loc_DifficultyLevel});
        }
        loc_inFile.close();
    }
    else{
        std::cerr<<"Score History not found, creating a new score history"<<std::endl;
    }
}

// void Cyclic(const int arg_Score, const int arg_DifficultyLevel)
// Description: Sets the scoreand difficulty level of the current player 
//              then update the vector _table
//              Display the scores in the console and then finally write
//              sorted _table to a file                            
// return type: void
// arguments  : const int arg_Score, const int arg_DifficultyLevel
void cPlayerInfo::Cyclic(const int arg_Score, const int arg_DifficultyLevel){
    _currPlayer._score = arg_Score;
    _currPlayer._difficultyLevel = arg_DifficultyLevel;

    UpdateTable();
    DisplayPlayerScoreHistory();
    WritePlayerScoreHistory();
}

// void UpdateTable()
// Description: Sets the current players name, score and difficulty level to 
//              the data member _table after the termination odf the game 
// return type: void
// arguments  : none
void cPlayerInfo::UpdateTable(){
     _table.push_back({_currPlayer._name, _currPlayer._score, _currPlayer._difficultyLevel});
}

// void SortPlayerScore()
// Description: Sorting the _table in descending order of score 
// return type: void
// arguments  : none
void cPlayerInfo::SortPlayerScore(){
    std::sort(_table.begin(), _table.end(), [](const sTable &a, const sTable &b){
        return a._score > b._score;
    });
}

// std::string DifficultyLevelInt2String()
// Description: This function will convert the integer difficulty level to string 
// return type: std::string
// arguments  : arg_difficultyLevel
std::string cPlayerInfo::DifficultyLevelInt2String(const int arg_difficultyLevel){
    std::string loc_retVal;
    switch(arg_difficultyLevel)
    {
        case 1:
         loc_retVal = "EASY";
         break;
        case 2:
         loc_retVal = "MEDIUM";
         break;
        case 3:
         loc_retVal = "HARD";
         break;
        default:
         loc_retVal = "INVALID INPUT";
         break;
    }
    return loc_retVal;
}

// void DisplayPlayerScoreHistory()
// Description: This function will  first sort the data member _table in descending order 
//              then display if the current player is the top scorer and the score table
// return type: void
// arguments  : none
void cPlayerInfo::DisplayPlayerScoreHistory(){
    std::cout<<_currPlayer._name<<" has scored "<<_currPlayer._score<<" in "<<DifficultyLevelInt2String(_currPlayer._difficultyLevel)<<" difficulty level."<<std::endl;
    SortPlayerScore();
    if(_currPlayer._score == _table.front()._score){
        std::cout<<"You are the Top Scorer!!"<<std::endl;
    }
    std::cout<<"NAME | SCORE | DIFFICULTY"<<std::endl;
    std::cout<<"*************************"<<std::endl;
    for(const auto &row : _table){
        std::cout<<row._name<<" | "<<row._score<<" | "<<DifficultyLevelInt2String(_currPlayer._difficultyLevel)<<std::endl;
        std::cout<<"---------------------------------------------"<<std::endl;
    }
}

// void WritePlayerScoreHistory()
// Description: Checks if the file ScoreSheet.txt exists, 
//              then write the new sorted _table 
//              else set the error message the file doesn't exist
// return type: void
// arguments  : none
void cPlayerInfo::WritePlayerScoreHistory(){ 
    std::ofstream loc_outFile("ScoreSheet.txt");
    if(loc_outFile.is_open()){
        for(const auto &row : _table){
            loc_outFile<<row._name<<' '<<row._score<<' '<<row._difficultyLevel<<'\n';
        }
        loc_outFile.close();
    }
    else{
        std::cerr<<"Score History not found, creating a new score history"<<std::endl;
    }
}
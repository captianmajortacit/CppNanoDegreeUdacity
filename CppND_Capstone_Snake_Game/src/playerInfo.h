#ifndef PLAYERINFO_H
#define PLAYERINFO_H


#include<string>
#include<vector>

struct sTable{
    std::string _name;
    int _score;
    int _difficultyLevel;
};

class cPlayerInfo{
    private:
    sTable _currPlayer;
    std::vector<sTable> _table;

    void        ReadPlayerScoreHistory();
    void        UpdateTable();
    void        SortPlayerScore();
    bool        CheckForTopScorer();
    std::string DifficultyLevelInt2String(const int arg_difficultyLevel);
    void        DisplayPlayerScoreHistory();
    void        WritePlayerScoreHistory();
   

    public:
    cPlayerInfo();
    void Cyclic(const int arg_Score, const int arg_DifficultyLevel);

};

#endif
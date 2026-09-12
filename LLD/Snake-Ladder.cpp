/*
•⁠ ⁠Design a snake and ladder game
 board - 1-N
 Jumper - snake and ladders, (extesible)
 Dice - 1-6 (dynamic - number of faces and number of dice)
 Players - N
 players starting position = 1
 Game - Start (reset board) -> running (start)
 Stop game criteria - allow every player to play until the end
 For future:
 * Moves for player - playmove, exitGame, timeout
 * EventBus, event bus listener
 */

Snake, Ladder inherit from BaseObject
Cell - contain information abt next position
enum GameState{
    INIT,
    RUNNING,
    ENDED
};
class Cell{
    uint id;
    vector<Object*> objList;
    uint getNextPos(){
        uint nextPos;
        for(auto obj : objList)
            nextPos = obj->getFinalPos();
        return nextPos;
    }
};
class Object{
    int startPos, endPos;
    uint getFinalPos(){}
};
class Snake : public Object{
};
class Ladder : public Object{
};
class Board{
    vector<Cell> cells;
    Board(vector<Object>& objects, int boardSize){
        cells = vector<Cell>(boardSize);
        for(auto& obj : objects){
            int start = obj.startPos;
            cells[start].objList.push_back(&obj);
        }
    }
    uint getNextPos(uint curPos){
        return cells[curPos].getNextPos();
    }
};
class Dice{
    size_t numFaces;
    Dice(size_t num){
        numfaces = num;
    }
    uint getNum() = 0;
};
class RandomDice : public Dice{
    uint getNum(){
        return random() % numFaces;
    }
};
class Player{
    int id;
    string name;
    uint curPos;
    Player(int id){
        id = id;
    }
};
class Game{
    Game(int boardSize, int DiceFaces, int numPlayers){
        dice = RandomDice(DiceFaces);
        board = Board(boardSize);
        for(int i=0;i<numPlayers;i++)
            playersQue.push(Player(i));
    }
    Board board;
    Dice dice = RandomDice();
    queue<Player> playersQue;
    queue<Player> winQue;
    while(!playersQue.empty()){
       int player = playersQue.front();
        int curPos = player.getPos();
        int num = dice.getNum();
        int nextPos = curPos + num;
        nextPos = board.getNextPos(nextPos);
        player.setPos(nextPos);
        if(board.checkWin(player))
            winQue.push(player);
        else
            playersQue.push(player);
        playersQue.pop();
    }
}

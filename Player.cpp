#include "Player.h"


Player::Player(GameMechs* thisGMRef, Food* thisFood)
{
    mainGameMechsRef = thisGMRef;
    myDir = STOP; // initial state of stop

    myFood = thisFood; //setting up the food reference

    // more actions to be included
    objPos tempPos;
    tempPos.setObjPos(mainGameMechsRef->getBoardSizeX()/2, 
                      mainGameMechsRef->getBoardSizeY()/2, 1); // sets character initilization

    playerPosList = new objPosArrayList();
    playerPosList->insertHead(tempPos);

}


Player::~Player()
{
    // delete any heap members here
    // empty for now, need in iteration 3
    delete playerPosList;

}

objPosArrayList* Player::getPlayerPos()
{
       // return the reference to the playerPos arrray list ... later
       return playerPosList;
}

void Player::updatePlayerDir()
{
    // PPA3 input processing logic
    //get inpuit from game mechanism class probably

    char input = mainGameMechsRef->getInput(); //use the get input from GameMech for input

    switch(input) 
    {
        case 'w':
                if (myDir != DOWN){
                    myDir = UP;
                }
                break;
            // Add more key processing here
            case 's':
                if (myDir != UP){
                    myDir = DOWN;
                }
                break;
            // Add more key processing here   
            case 'a':
                if (myDir != RIGHT){
                    myDir = LEFT;
                }
                break;
                

            case 'd':
                if (myDir != LEFT){
                    myDir = RIGHT;
                }
            default:
                break;

    }

}

bool Player::checkFoodConsumption(objPos snakeHead, objPos foodPos)
{
    return foodPos.isPosEqual(&snakeHead);
}

void Player::increasePlayerLength(objPos snakeHead)
{
    playerPosList->insertHead(snakeHead);
    myFood->generateFood(playerPosList);
    mainGameMechsRef->incrementScore(playerPosList);
}

bool Player::checkSelfCollision(objPos snakeHead)
{
    objPos tempPos;

    for(int i = 1; i < playerPosList->getSize(); i++)
    {
        playerPosList->getElement(tempPos, i);
        if(snakeHead.isPosEqual(&tempPos))
        {
            return true; //if theres a collision, return true
        }
    }
    return false; //if theres not a collision, return false
}

void Player::movePlayer()
{
    objPos currentHead; //holds pos of current head
    playerPosList->getHeadElement(currentHead);

    //stores the position of the food into foodPos
    objPos foodPos;
    myFood->getFoodPos(foodPos);

    // PPA3 Finite State Machine logic
    if(myDir == RIGHT)
    {
        currentHead.x += 1;
    }
    else if (myDir == LEFT)
    {
        currentHead.x -= 1;
    }
    else if (myDir == DOWN)
    {
        currentHead.y += 1;
    }
    else if (myDir == UP)
    {
        currentHead.y -= 1;
    }

    //border wrap around
    if (currentHead.x == mainGameMechsRef->getBoardSizeX() - 1)
    {
        currentHead.x = 1;
    }
    if (currentHead.y == mainGameMechsRef->getBoardSizeY() - 1)
    {
        currentHead.y = 1;
    }
    if (currentHead.x == 0)
    {
        currentHead.x = mainGameMechsRef->getBoardSizeX() - 2;
    }
    if (currentHead.y == 0)
    {
        currentHead.y = mainGameMechsRef->getBoardSizeY() - 2;
    }
    
    //updates player posistion based on logic.

    //new current head should inserted to head of list
    //remove tail from list
    if(checkFoodConsumption(currentHead, foodPos))
    {
       increasePlayerLength(currentHead);
    }
    else
    {
        playerPosList->insertHead(currentHead);
        playerPosList->removeTail();
    }
    if(checkSelfCollision(currentHead))
    {
        mainGameMechsRef->setLoseFlag();
        mainGameMechsRef->setExitTrue();
    }
}
//
//  GameScene.hpp
//  ColorSort-desktop
//
//  Created by hoangbui-fe on 8/8/26.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "cocos2d.h"
#include "FallingBlock.hpp"

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    CREATE_FUNC(GameScene);

private:
    enum class GameState
    {
        Playing,
        GameOver,
        Won
    };

    void setupBackground();
    void setupBins();
    void setupHUD();
    void setupInput();

    void createBin(
        float x,
        const cocos2d::Color4F& color
    );

    void spawnBlock();

    void update(float delta) override;

    void updateBlocks(float delta);
    void checkBlockCollisions();

    void handleMouseDown(cocos2d::EventMouse* event);
    void handleMouseMove(cocos2d::EventMouse* event);
    void handleMouseUp(cocos2d::EventMouse* event);

    void handleKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    bool tryGrabBlock(const cocos2d::Vec2& mousePosition);

    bool isBlockInsideBin(
        FallingBlock* block,
        const cocos2d::Rect& binRect
    ) const;

    BlockColor getBinColor(
        const cocos2d::Rect& binRect
    ) const;

    void sortBlock(FallingBlock* block);
    void gameOver();
    void winGame();
    void resetGame();

    void updateScoreLabel();
    void showGameMessage(
        const std::string& message,
        const cocos2d::Color3B& color
    );

    cocos2d::Rect getRedBinRect() const;
    cocos2d::Rect getGreenBinRect() const;
    cocos2d::Rect getBlueBinRect() const;

private:
    GameState _gameState = GameState::Playing;

    int _score = 0;

    float _spawnTimer = 0.0f;

    cocos2d::Vector<FallingBlock*> _blocks;

    FallingBlock* _grabbedBlock = nullptr;

    cocos2d::Label* _scoreLabel = nullptr;
    cocos2d::Label* _gameMessageLabel = nullptr;

    cocos2d::EventListenerMouse* _mouseListener = nullptr;
    cocos2d::EventListenerKeyboard* _keyboardListener = nullptr;
};

#endif /* GameScene_hpp */

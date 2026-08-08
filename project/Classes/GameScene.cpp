//
//  GameScene.cpp
//  ColorSort-desktop
//
//  Created by hoangbui-fe on 8/8/26.
//

#include "GameScene.hpp"
#include "Constants.h"

#include <algorithm>
#include <random>

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    setupBackground();
    setupBins();
    setupHUD();
    setupInput();

    scheduleUpdate();

    return true;
}

void GameScene::setupBackground()
{
    auto background = LayerColor::create(
        Color4B(42, 42, 42, 255),
        ColorSort::DESIGN_WIDTH,
        ColorSort::DESIGN_HEIGHT
    );

    background->setPosition(Vec2::ZERO);

    addChild(background, -100);
}

void GameScene::setupBins()
{
    createBin(
        ColorSort::RED_BIN_X,
        Color4F(230.0f / 255.0f, 30.0f / 255.0f, 30.0f / 255.0f, 1.0f)
    );

    createBin(
        ColorSort::GREEN_BIN_X,
        Color4F(40.0f / 255.0f, 200.0f / 255.0f, 60.0f / 255.0f, 1.0f)
    );

    createBin(
        ColorSort::BLUE_BIN_X,
        Color4F(45.0f / 255.0f, 90.0f / 255.0f, 220.0f / 255.0f, 1.0f)
    );
}

void GameScene::createBin(
    float x,
    const Color4F& color
)
{
    auto bin = DrawNode::create();

    bin->drawSolidRect(
        Vec2::ZERO,
        Vec2(
            ColorSort::BIN_WIDTH,
            ColorSort::BIN_HEIGHT
        ),
        color
    );

    bin->setPosition(
        Vec2(
            x,
            ColorSort::BIN_Y
        )
    );

    addChild(bin, 0);
}

void GameScene::setupHUD()
{
    _scoreLabel = Label::createWithTTF(
        "Score: 0",
        "fonts/arial.ttf",
        ColorSort::SCORE_FONT_SIZE
    );

    _scoreLabel->setAnchorPoint(Vec2(0.0f, 1.0f));

    _scoreLabel->setPosition(
        Vec2(
            20.0f,
            ColorSort::DESIGN_HEIGHT - 20.0f
        )
    );

    addChild(_scoreLabel, 100);

    _gameMessageLabel = Label::createWithTTF(
        "",
        "fonts/arial.ttf",
        ColorSort::MESSAGE_FONT_SIZE
    );

    _gameMessageLabel->setAnchorPoint(Vec2(0.5f, 0.5f));

    _gameMessageLabel->setPosition(
        Vec2(
            ColorSort::DESIGN_WIDTH * 0.5f,
            ColorSort::DESIGN_HEIGHT * 0.5f
        )
    );

    _gameMessageLabel->setVisible(false);

    addChild(_gameMessageLabel, 100);
}

void GameScene::setupInput()
{
    _mouseListener = EventListenerMouse::create();

    _mouseListener->onMouseDown =
        CC_CALLBACK_1(
            GameScene::handleMouseDown,
            this
        );

    _mouseListener->onMouseMove =
        CC_CALLBACK_1(
            GameScene::handleMouseMove,
            this
        );

    _mouseListener->onMouseUp =
        CC_CALLBACK_1(
            GameScene::handleMouseUp,
            this
        );

    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        _mouseListener,
        this
    );

    _keyboardListener = EventListenerKeyboard::create();

    _keyboardListener->onKeyPressed =
        CC_CALLBACK_2(
            GameScene::handleKeyPressed,
            this
        );

    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        _keyboardListener,
        this
    );
}

void GameScene::update(float delta)
{
    if (_gameState != GameState::Playing)
    {
        return;
    }

    _spawnTimer += delta;

    while (_spawnTimer >= ColorSort::SPAWN_INTERVAL)
    {
        _spawnTimer -= ColorSort::SPAWN_INTERVAL;

        spawnBlock();
    }

    updateBlocks(delta);
    checkBlockCollisions();
}

void GameScene::spawnBlock()
{
    const float randomX = RandomHelper::random_real(ColorSort::SPAWN_MIN_X, ColorSort::SPAWN_MAX_X);

    BlockColor color;

    const int colorIndex = RandomHelper::random_int(0, 2);

    switch (colorIndex)
    {
        case 0:
            color = BlockColor::Red;
            break;

        case 1:
            color = BlockColor::Green;
            break;

        default:
            color = BlockColor::Blue;
            break;
    }

    auto block = FallingBlock::create(color);

    if (block == nullptr)
    {
        return;
    }

    block->setPosition(
        Vec2(
            randomX,
            ColorSort::SPAWN_Y
        )
    );

    addChild(block, 10);

    _blocks.pushBack(block);
}

void GameScene::updateBlocks(float delta)
{
    for (auto block : _blocks)
    {
        if (block == nullptr)
        {
            continue;
        }

        if (block == _grabbedBlock)
        {
            continue;
        }

        if (block->isGrabbed())
        {
            continue;
        }

        block->setPositionY(
            block->getPositionY() -
            ColorSort::BLOCK_FALL_SPEED * delta
        );
    }
}

void GameScene::checkBlockCollisions()
{
    for (auto block : _blocks)
    {
        if (block == nullptr)
        {
            continue;
        }

        if (block == _grabbedBlock)
        {
            continue;
        }

        if (block->isGrabbed())
        {
            continue;
        }

        const Rect blockRect =
            block->getBoundingBoxInScene();

        const Rect redBin = getRedBinRect();
        const Rect greenBin = getGreenBinRect();
        const Rect blueBin = getBlueBinRect();

        if (blockRect.intersectsRect(redBin))
        {
            sortBlock(block);
            return;
        }

        if (blockRect.intersectsRect(greenBin))
        {
            sortBlock(block);
            return;
        }

        if (blockRect.intersectsRect(blueBin))
        {
            sortBlock(block);
            return;
        }

        /*
         * The block is considered missed only after its
         * complete bounding box has moved below the screen.
         */
        if (blockRect.getMaxY() < 0.0f)
        {
            gameOver();
            return;
        }
    }
}

void GameScene::handleMouseDown(EventMouse* event)
{
    if (_gameState != GameState::Playing)
    {
        return;
    }

    if (event->getMouseButton() != EventMouse::MouseButton::BUTTON_LEFT)
    {
        return;
    }

    const Vec2 mousePosition(
        event->getCursorX(),
        event->getCursorY()
    );

    tryGrabBlock(mousePosition);
}

void GameScene::handleMouseMove(EventMouse* event)
{
    if (_gameState != GameState::Playing)
    {
        return;
    }

    if (_grabbedBlock == nullptr)
    {
        return;
    }

    const Vec2 mousePosition(
        event->getCursorX(),
        event->getCursorY()
    );

    /*
     * The block's position represents its center, so
     * directly assigning the mouse position makes the
     * block's center rigidly follow the cursor.
     */
    _grabbedBlock->setPosition(mousePosition);
}

void GameScene::handleMouseUp(EventMouse* event)
{
    if (_gameState != GameState::Playing)
    {
        return;
    }

    if (event->getMouseButton() != EventMouse::MouseButton::BUTTON_LEFT)
    {
        return;
    }

    if (_grabbedBlock == nullptr)
    {
        return;
    }

    _grabbedBlock->setGrabbed(false);
    _grabbedBlock = nullptr;

    /*
     * Check immediately after releasing. This allows a
     * dropped block already overlapping a bin to be
     * resolved on the same frame.
     */
    checkBlockCollisions();
}

bool GameScene::tryGrabBlock(
    const Vec2& mousePosition
)
{
    /*
     * Iterate from the end so the visually topmost block
     * is selected if blocks overlap.
     */
    for (int i = static_cast<int>(_blocks.size()) - 1;
         i >= 0;
         --i)
    {
        auto block = _blocks.at(i);

        if (block == nullptr)
        {
            continue;
        }

        if (block->isGrabbed())
        {
            continue;
        }

        const Rect blockRect =
            block->getBoundingBoxInScene();

        if (blockRect.containsPoint(mousePosition))
        {
            _grabbedBlock = block;

            block->setGrabbed(true);

            /*
             * The specification requires the center to
             * rigidly follow the mouse, so there is no
             * cursor-to-center offset.
             */
            block->setPosition(mousePosition);

            return true;
        }
    }

    return false;
}

bool GameScene::isBlockInsideBin(
    FallingBlock* block,
    const Rect& binRect
) const
{
    if (block == nullptr)
    {
        return false;
    }

    return block->getBoundingBoxInScene()
        .intersectsRect(binRect);
}

BlockColor GameScene::getBinColor(
    const Rect& binRect
) const
{
    if (binRect.equals(getRedBinRect()))
    {
        return BlockColor::Red;
    }

    if (binRect.equals(getGreenBinRect()))
    {
        return BlockColor::Green;
    }

    return BlockColor::Blue;
}

void GameScene::sortBlock(FallingBlock* block)
{
    if (block == nullptr)
    {
        return;
    }

    const Rect blockRect =
        block->getBoundingBoxInScene();

    Rect matchingBin;

    if (blockRect.intersectsRect(getRedBinRect()))
    {
        matchingBin = getRedBinRect();
    }
    else if (blockRect.intersectsRect(getGreenBinRect()))
    {
        matchingBin = getGreenBinRect();
    }
    else if (blockRect.intersectsRect(getBlueBinRect()))
    {
        matchingBin = getBlueBinRect();
    }
    else
    {
        return;
    }

    const BlockColor binColor =
        getBinColor(matchingBin);

    if (block->getBlockColor() != binColor)
    {
        gameOver();
        return;
    }

    _blocks.eraseObject(block);

    if (_grabbedBlock == block)
    {
        _grabbedBlock = nullptr;
    }

    block->removeFromParent();

    ++_score;

    updateScoreLabel();

    if (_score >= ColorSort::WIN_SCORE)
    {
        winGame();
    }
}

void GameScene::gameOver()
{
    if (_gameState != GameState::Playing)
    {
        return;
    }

    _gameState = GameState::GameOver;

    if (_grabbedBlock != nullptr)
    {
        _grabbedBlock->setGrabbed(false);
        _grabbedBlock = nullptr;
    }

    showGameMessage(
        "GAME OVER!",
        Color3B::RED
    );
}

void GameScene::winGame()
{
    if (_gameState != GameState::Playing)
    {
        return;
    }

    _gameState = GameState::Won;

    if (_grabbedBlock != nullptr)
    {
        _grabbedBlock->setGrabbed(false);
        _grabbedBlock = nullptr;
    }

    showGameMessage(
        "YOU WIN!",
        Color3B::GREEN
    );
}

void GameScene::resetGame()
{
    _gameState = GameState::Playing;

    _score = 0;
    _spawnTimer = 0.0f;

    _grabbedBlock = nullptr;

    for (auto block : _blocks)
    {
        if (block != nullptr)
        {
            block->removeFromParent();
        }
    }

    _blocks.clear();

    updateScoreLabel();

    _gameMessageLabel->setString("");
    _gameMessageLabel->setVisible(false);
}

void GameScene::updateScoreLabel()
{
    if (_scoreLabel == nullptr)
    {
        return;
    }

    _scoreLabel->setString(
        StringUtils::format(
            "Score: %d",
            _score
        )
    );
}

void GameScene::showGameMessage(
    const std::string& message,
    const Color3B& color
)
{
    if (_gameMessageLabel == nullptr)
    {
        return;
    }

    _gameMessageLabel->setString(message);
    _gameMessageLabel->setColor(color);
    _gameMessageLabel->setVisible(true);
}

void GameScene::handleKeyPressed(
    EventKeyboard::KeyCode keyCode, Event* event
)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_R)
    {
        resetGame();
        return;
    }

    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
    {
        Director::getInstance()->end();
    }
}

Rect GameScene::getRedBinRect() const
{
    return Rect(
        ColorSort::RED_BIN_X,
        ColorSort::BIN_Y,
        ColorSort::BIN_WIDTH,
        ColorSort::BIN_HEIGHT
    );
}

Rect GameScene::getGreenBinRect() const
{
    return Rect(
        ColorSort::GREEN_BIN_X,
        ColorSort::BIN_Y,
        ColorSort::BIN_WIDTH,
        ColorSort::BIN_HEIGHT
    );
}

Rect GameScene::getBlueBinRect() const
{
    return Rect(
        ColorSort::BLUE_BIN_X,
        ColorSort::BIN_Y,
        ColorSort::BIN_WIDTH,
        ColorSort::BIN_HEIGHT
    );
}

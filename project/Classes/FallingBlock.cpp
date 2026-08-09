//
//  FallingBlock.cpp
//  ColorSort-desktop
//
//  Created by hoangbui-fe on 8/8/26.
//

#include "FallingBlock.hpp"
#include "Constants.h"

USING_NS_CC;

FallingBlock* FallingBlock::create(BlockColor color)
{
    auto block = new (std::nothrow) FallingBlock();

    if (block && block->init(color))
    {
        block->autorelease();
        return block;
    }

    delete block;
    return nullptr;
}

bool FallingBlock::init(BlockColor color)
{
    if (!Node::init())
    {
        return false;
    }

    _color = color;

    _drawNode = DrawNode::create();

    _drawNode->drawSolidRect(
        Vec2::ZERO,
        Vec2(
            ColorSort::BLOCK_SIZE,
            ColorSort::BLOCK_SIZE
        ),
        Color4F(getDisplayColor(_color))
    );

    addChild(_drawNode, kChildTagDrawNode);

    setContentSize(
        Size(
            ColorSort::BLOCK_SIZE,
            ColorSort::BLOCK_SIZE
        )
    );

    setAnchorPoint(Vec2(0.5f, 0.5f));

    return true;
}

BlockColor FallingBlock::getBlockColor() const
{
    return _color;
}

Rect FallingBlock::getBoundingBoxInScene() const
{
    return Rect(
        getPositionX() - ColorSort::BLOCK_HALF_SIZE,
        getPositionY() - ColorSort::BLOCK_HALF_SIZE,
        ColorSort::BLOCK_SIZE,
        ColorSort::BLOCK_SIZE
    );
}

void FallingBlock::setGrabbed(bool grabbed)
{
    _grabbed = grabbed;
}

bool FallingBlock::isGrabbed() const
{
    return _grabbed;
}

Color3B FallingBlock::getDisplayColor(BlockColor color)
{
    switch (color)
    {
        case BlockColor::Red:
            return Color3B(230, 30, 30);

        case BlockColor::Green:
            return Color3B(40, 200, 60);

        case BlockColor::Blue:
            return Color3B(45, 90, 220);
    }

    return Color3B::WHITE;
}

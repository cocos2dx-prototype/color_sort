//
//  FallingBlock.hpp
//  ColorSort-desktop
//
//  Created by hoangbui-fe on 8/8/26.
//

#ifndef FallingBlock_hpp
#define FallingBlock_hpp

#include "cocos2d.h"

enum class BlockColor
{
    Red = 0,
    Green,
    Blue
};

class FallingBlock : public cocos2d::Node {
    
public:
    static FallingBlock* create(BlockColor color);

    bool init(BlockColor color);

    BlockColor getBlockColor() const;

    cocos2d::Rect getBoundingBoxInScene() const;

    void setGrabbed(bool grabbed);
    bool isGrabbed() const;

    static cocos2d::Color3B getDisplayColor(BlockColor color);

private:
    BlockColor _color = BlockColor::Red;
    bool _grabbed = false;

    cocos2d::DrawNode* _drawNode = nullptr;
};

#endif /* FallingBlock_hpp */

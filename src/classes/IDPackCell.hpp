#include <cocos2d.h>
#include <span>

class IDPackCell : public cocos2d::CCLayer {
public:
    static IDPackCell* create(std::string_view, double, std::span<const int>, std::string_view);

    void draw() override;
protected:
    cocos2d::CCSprite* m_background;
    std::vector<cocos2d::ccColor4F> m_colors;
    std::span<const int> m_levels;
    int m_colorMode = 0;

    bool init(std::string_view, double, std::span<const int>, std::string_view);
    void onClick(cocos2d::CCObject*);
};

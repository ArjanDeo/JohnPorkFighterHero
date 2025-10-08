#pragma once
#include "UIElement.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "MissionSelection.h"




class MissionCard : public UIElement {
public:
    MissionCard(const sf::Font& font, const MissionSelection& mission);

    void setPosition(const sf::Vector2f& pos) override;
    void setSize(const sf::Vector2f& size) override;
    void setSelected(bool selected);
    bool isSelected() const { return selected; }
    int getMissionId() const { return missionId; }

    // UIElement implementation
    void handleEvent(const std::optional<sf::Event>& event) override;
    void update(float dt) override {}
    void draw(sf::RenderWindow& window) override;
    bool isPointInside(int pointX, int pointY) const override;

private:
    void updateLayout();
    sf::Color getDifficultyColor(MissionDifficulty difficulty) const;
    std::string getDifficultyText(MissionDifficulty difficulty) const;

    // Visual components
    sf::RectangleShape cardShape;
    sf::Sprite thumbnailSprite;
    sf::Texture thumbnailTexture;
    sf::Text titleText;
    sf::Text descriptionText;
    sf::Text difficultyText;
    sf::RectangleShape difficultyBadge;

    // Data
    int missionId;
    bool selected;
    bool isHovered;

    // Colors
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color selectedColor;

    // Layout constants
    static const int THUMBNAIL_WIDTH;
    static const int THUMBNAIL_HEIGHT;
    static const float PADDING;
    static const float TEXT_SPACING;
};
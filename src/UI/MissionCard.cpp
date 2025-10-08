#include "UI/MissionCard.h"
#include "Common.h"
#include <iostream>
// Layout constants
const int MissionCard::THUMBNAIL_WIDTH = 300.0;
const int MissionCard::THUMBNAIL_HEIGHT = 200.0;
const float MissionCard::PADDING = 15.0f;
const float MissionCard::TEXT_SPACING = 8.0f;

MissionCard::MissionCard(const sf::Font& font, const MissionSelection& mission)
    : UIElement(0, 0, 700, 300), missionId(mission.missionId), selected(false), isHovered(false), titleText(font), descriptionText(font), difficultyText(font), thumbnailSprite(mission.thumbnailTexture){

    // Set up colors
    normalColor = sf::Color(45, 45, 50);
    hoverColor = sf::Color(60, 60, 70);
    selectedColor = sf::Color(80, 120, 180);

    // Set up card background
    cardShape.setSize({ 700.0f, 300.0f });
    cardShape.setFillColor(normalColor);
    cardShape.setOutlineThickness(2.0f);
    cardShape.setOutlineColor(sf::Color(80, 80, 90));

    // Set up thumbnail
    thumbnailSprite.setScale(Common::ScaleVector(mission.thumbnailTexture.getSize(), { THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT }));
    
    // Set up title text
    titleText.setFont(font);
    titleText.setString(mission.missionName);
    titleText.setCharacterSize(18);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    // Set up description text
    descriptionText.setFont(font);
    descriptionText.setString(mission.missionDescription);
    descriptionText.setCharacterSize(12);
    descriptionText.setFillColor(sf::Color(200, 200, 200));

    // Set up difficulty badge and text
    difficultyBadge.setSize({ 60.0f, 20.0f });
    difficultyBadge.setFillColor(getDifficultyColor(mission.difficulty));

    difficultyText.setFont(font);
    difficultyText.setString(getDifficultyText(mission.difficulty));
    difficultyText.setCharacterSize(10);
    difficultyText.setFillColor(sf::Color::White);
    difficultyText.setStyle(sf::Text::Bold);

    updateLayout();
}

void MissionCard::setPosition(const sf::Vector2f& pos) {
    UIElement::setPosition(pos);
    cardShape.setPosition(pos);
    updateLayout();
}

void MissionCard::setSize(const sf::Vector2f& size) {
    UIElement::setSize(size);
    cardShape.setSize(size);
    updateLayout();
}

void MissionCard::setSelected(bool sel) {
    selected = sel;
    if (selected) {
        cardShape.setFillColor(selectedColor);
        cardShape.setOutlineColor(sf::Color(120, 160, 220));
        cardShape.setOutlineThickness(3.0f);
    }
    else if (isHovered) {
        cardShape.setFillColor(hoverColor);
        cardShape.setOutlineColor(sf::Color(100, 100, 120));
        cardShape.setOutlineThickness(2.0f);
    }
    else {
        cardShape.setFillColor(normalColor);
        cardShape.setOutlineColor(sf::Color(80, 80, 90));
        cardShape.setOutlineThickness(2.0f);
    }
}

bool MissionCard::isPointInside(int pointX, int pointY) const {
    sf::Vector2f mousePos(static_cast<float>(pointX), static_cast<float>(pointY));
    return cardShape.getGlobalBounds().contains(mousePos);
}

void MissionCard::handleEvent(const std::optional<sf::Event>& event) {
    if (!event.has_value() || !visible) return;

    // Handle hover effects
    if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePos(static_cast<float>(mouseMoved->position.x),
            static_cast<float>(mouseMoved->position.y));
        bool wasHovered = isHovered;
        isHovered = isPointInside(mousePos.x, mousePos.y);

        if (isHovered != wasHovered) {
            cardShape.setFillColor(isHovered ? hoverColor : normalColor);
        }
    }

    // Call parent to handle click logic
    UIElement::handleEvent(event);
}

void MissionCard::draw(sf::RenderWindow& window) {
    if (!visible) return;

    window.draw(cardShape);
    window.draw(thumbnailSprite);
    window.draw(titleText);
    window.draw(descriptionText);
    window.draw(difficultyBadge);
    window.draw(difficultyText);
}

void MissionCard::updateLayout() {
    sf::Vector2f cardPos = cardShape.getPosition();
    sf::Vector2f cardSize = cardShape.getSize();

    // Position thumbnail on the center
    thumbnailSprite.setPosition(
        { cardPos.x + (cardSize.x - THUMBNAIL_WIDTH),
        cardPos.y + (cardSize.y - THUMBNAIL_HEIGHT) / 2.0f }
    );

    // Position title text
    float textX = cardPos.x + PADDING + THUMBNAIL_WIDTH + PADDING;
    titleText.setPosition({ textX, cardPos.y + PADDING });

    // Position description text below title
    sf::FloatRect titleBounds = titleText.getGlobalBounds();
    descriptionText.setPosition(
        { textX,
        titleBounds.position.y + titleBounds.size.y + TEXT_SPACING }
    );

    // Position difficulty badge in bottom right
    difficultyBadge.setPosition(
        { cardPos.x + cardSize.x - 70.0f,
        cardPos.y + cardSize.y - 30.0f }
    );

    // Center difficulty text in badge
    sf::FloatRect badgeBounds = difficultyBadge.getGlobalBounds();
    sf::FloatRect diffTextBounds = difficultyText.getLocalBounds();
    difficultyText.setPosition(
        { badgeBounds.position.x + (badgeBounds.size.x - diffTextBounds.size.x) / 2.0f,
        badgeBounds.position.y + (badgeBounds.size.y - diffTextBounds.size.y) / 2.0f - 2.0f }
    );
}

sf::Color MissionCard::getDifficultyColor(MissionDifficulty difficulty) const {
    switch (difficulty) {
    case EASY:   return sf::Color(80, 150, 80);   // Green
    case MEDIUM: return sf::Color(200, 160, 60);  // Yellow/Orange
    case HARD:   return sf::Color(180, 60, 60);   // Red
    default:     return sf::Color(100, 100, 100); // Gray
    }
}

std::string MissionCard::getDifficultyText(MissionDifficulty difficulty) const {
    switch (difficulty) {
    case EASY:   return "EASY";
    case MEDIUM: return "MEDIUM";
    case HARD:   return "HARD";
    default:     return "UNKNOWN";
    }
}
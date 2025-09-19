#include "Game.h"
#include "Player.h"
#include "Button.h"
#include "Tree.h"
#include "Item.h"
#include "RecourceManager.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <memory>

enum class Type;

void Game::initObjects() {
    //Player
    auto playerObj = std::make_unique<Player>(
        resources.getTexture("player_walk"),
        resources.getTexture("player_idle"),
        sf::Vector2f(window.getSize().x/2.f-32.f, window.getSize().y/2.f-32.f),
        300.f
    );
    player = playerObj.get();
    gameObjects.push_back(std::move(playerObj));
}

void Game::initTextures() {
    resources.loadTexture("player_walk", "assets/used/Slime1_Walk_full.png");
    resources.loadTexture("player_idle", "assets/used/Slime1_Idle_full.png");
    resources.loadTexture("portal", "assets/used/Portals.png");
    resources.loadTexture("background", "assets/used/grassbg.png");
    resources.loadTexture("tree", "assets/used/tree.png");
    resources.loadTexture("trunk", "assets/used/stump.png");
    resources.loadTexture("wood", "assets/used/wood.png");
    resources.loadTexture("sapling", "assets/used/placeholder.png");
    resources.loadTexture("apple", "assets/used/placeholder2.png");
}

void Game::initTrees() {
    const std::string filename = "gamesave.txt";
    std::ifstream in(filename);
    if (in.is_open()) {
        in.close();
        loadFromFile(filename);
    } else {
        std::cout << "No save file found, generating new trees." << std::endl;
        static std::mt19937 rng(std::random_device{}());

        float areaSize = 2000.f;
        int treeCount = 50;
        float minDist = 100.f; // minimum distance between trees and player start

        std::uniform_real_distribution<float> dist(0, areaSize);

        //get player start position
        sf::Vector2f playerPos(0.f, 0.f);
        for (auto& obj : gameObjects) {
            if (auto* p = dynamic_cast<Player*>(obj.get())) {
                playerPos = p->getPosition(); break;
            }
        }

        std::vector<sf::Vector2f> positions;
        int tries = 0;
        int maxTries = treeCount * 10; // prevent infinite loop

        while ((int)positions.size() < treeCount && tries < maxTries) {
        sf::Vector2f pos(dist(rng), dist(rng));
        bool tooClose = false;

        // Abstand zu Spieler prüfen
        float dxP = pos.x - playerPos.x;
        float dyP = pos.y - playerPos.y;
        if (std::sqrt(dxP*dxP + dyP*dyP) < minDist) {
            tooClose = true;
        }

        // Abstand zu anderen Bäumen prüfen
        for (const auto& other : positions) {
            float dx = pos.x - other.x;
            float dy = pos.y - other.y;
            if (std::sqrt(dx*dx + dy*dy) < minDist) {
                tooClose = true;
                break;
            }
        }

        if (!tooClose) {
            positions.push_back(pos);
        }

        ++tries;
    }
    
    //sort trees by y for better rendering order
    std::sort(positions.begin(), positions.end(), [](const sf::Vector2f& a, const sf::Vector2f& b) {
        return (a.y < b.y) || (a.y == b.y && a.x < b.x);
    });

    for (const auto& pos : positions) {
        auto tree = std::make_unique<Tree>(pos, resources.getTexture("tree"), resources.getTexture("trunk"), resources.getTexture("wood"), resources.getTexture("sapling"), resources.getTexture("apple")); 
        visibleTrees.push_back(std::move(tree));
    }

    saveToFile(filename);
    }
}

void Game::loadFromFile(const std::string& filename) {
    visibleTrees.clear();
    saplings.clear();
    if (player) player->getInventory().clear();

    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Could not open " << filename << " for reading!" << std::endl;
        return;
    }

    std::string type;
    while (in >> type) {
        if (type == "PLAYER") {
            float x, y;
            in >> x >> y;
            if (player) player->setPosition(sf::Vector2f(x, y));
            //TODO load player stats
        }else if (type == "ITEM") {
            std::string name;
            int typeInt, quantity;
            in >> name >> typeInt >> quantity;
            if (player) {
                Item item(name, static_cast<Type>(typeInt), quantity, true, &resources.getTexture(name), sf::Vector2f(0,0));
                player->getInventory().addItem(item);
            }
        } else if (type == "TREE") {
            float x, y;
            in >> x >> y;
            auto tree = std::make_unique<Tree>(
                sf::Vector2f(x, y),
                resources.getTexture("tree"),
                resources.getTexture("trunk"),
                resources.getTexture("wood"),
                resources.getTexture("sapling"),
                resources.getTexture("apple")
            );
            visibleTrees.push_back(std::move(tree));
        } else if (type == "SAPLING") {
            float x, y;
            int state;
            in >> x >> y >> state;
            saplings.push_back(std::make_unique<Sapling>(
                sf::Vector2f(x, y),
                resources.getTexture("sapling"),
                state
            ));
        }
    }
    in.close();
    std::cout << "Game loaded from " << filename << std::endl;
}

void Game::saveToFile(const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Could not open " << filename << " for writing!" << std::endl;
        return;
    }

    if (player) {
        sf::Vector2f pos = player->getPosition();
        out << "PLAYER " << pos.x << " " << pos.y << "\n";

        // TODO Save player stats

        // Save Inventory
        for (const auto& item : player->getInventory().getItems()) {
            out << "ITEM " << item->getName() << " " << static_cast<int>(item->getType()) << " " << item->getQuantity() << "\n";
        }
    }

    // Save trees
    std::vector<sf::Vector2f> treePositions;
    for (const auto& tree : visibleTrees) {
        treePositions.push_back(tree->getPosition());
    }
    std::sort(treePositions.begin(), treePositions.end(), [](const sf::Vector2f& a, const sf::Vector2f& b) {
        return (a.y < b.y) || (a.y == b.y && a.x < b.x);
    });

    for (const auto& pos : treePositions) {
        out << "TREE " << pos.x << " " << pos.y << "\n";
    }

    // Save saplings
    for (const auto& sapling : saplings) {
        sf::Vector2f pos = sapling->getPosition();
        int state = sapling->getGrowthState();
        out << "SAPLING " << pos.x << " " << pos.y << " " << state << "\n";
    }

    out.close();
    std::cout << "Game saved to " << filename << std::endl;
}

Game::Game() : portals() {
    initWindow();
    initTextures();
    initBackground();
    initObjects();

    camera.setSize(window.getSize().x, window.getSize().y);
    uiView.setSize(window.getSize().x, window.getSize().y);
    uiView.setCenter(window.getSize().x/2.f, window.getSize().y/2.f);

    initTrees();
    portals.setTexture(resources.getTexture("portal"));
    //std::cout << __PRETTY_FUNCTION__ << std::endl; //debug
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::initWindow() {
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    window.create(mode, "Karl", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    //std::cout << __PRETTY_FUNCTION__ << std::endl; // debug
}

void Game::initBackground() {
    backgroundSprite.setTexture(resources.getTexture("background"));
    backgroundSprite.setScale(4, 4);
    //std::cout << __PRETTY_FUNCTION__ << std::endl; //debug
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        // --- KEYS ---
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            } else if (event.key.code == sf::Keyboard::Tab) {
                player->getInventory().toggleVisibility();
            }

        // --- MOUSE ---
        } else if(event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos, camera);
            sf::Vector2f uiMouse = window.mapPixelToCoords(mousePos, uiView);

            bool handled = false;

            // UI interaction first
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (player->getInventory().isVisible() && player->getInventory().containsPoint(uiMouse)) {
                    player->getInventory().handleClick(uiMouse);
                    handled = true;
                }
                // put button logic here
            }

            // World interactions if not handled by ui
            if(!handled && event.mouseButton.button == sf::Mouse::Left) {
                //chop trees
                for (auto& tree : visibleTrees) {
                    if (tree->getGlobalBounds().contains(mouseWorld)) {
                        tree->chop(worldItems);
                        handled = true;
                        break;
                    }
                }

                //plant saplings
                if (!handled && player->getInventory().getResourceQuantity(Type::SAPLING) > 0) {
                    saplings.push_back(std::make_unique<Sapling>(mouseWorld, resources.getTexture("sapling")));
                    player->getInventory().removeItem(Type::SAPLING, 1);
                    handled = true;
                }

                //place portal when no other element was clicked on
                if(!handled){
                    portals.handleInput(mouseWorld, event.mouseButton.button);
                }   
            }                     
        } else if(event.type == sf::Event::Closed) {
            window.close(); 
        } 
    }
}

void Game::checkItemPickup() {
    if (!player) return;

    Inventory& inv = player->getInventory();
    Hotbar& hotbar = player->getHotbar();

    for (auto it = worldItems.begin(); it != worldItems.end();) {
        Item& item = *(*it);

        // 1. Stacken im Inventar, falls Item schon existiert
        bool stacked = false;
        for (auto& invItem : inv.getItems()) {
            if (invItem->getType() == item.getType()) {
                invItem->addQuantity(item.getQuantity());
                // Hotbar-Slot auf dieses Item setzen, falls noch nicht vorhanden
                bool inHotbar = false;
                for (int i = 0; i < hotbar.getSlotCount(); ++i) {
                    if (hotbar.getSlot(i) == invItem.get()) {
                        inHotbar = true;
                        break;
                    }
                }
                if (!inHotbar) {
                    // Freien Slot suchen
                    for (int i = 0; i < hotbar.getSlotCount(); ++i) {
                        if (hotbar.getSlot(i) == nullptr) {
                            hotbar.setSlot(i, invItem.get());
                            break;
                        }
                    }
                }
                stacked = true;
                break;
            }
        }
        if (stacked) {
            it = worldItems.erase(it);
            continue;
        }

        // 2. Item existiert noch nicht: Prüfe, ob Inventar Platz hat
        if (inv.getItems().size() < inv.getCapacity()) {
            // Freien Hotbar-Slot suchen
            bool placedInHotbar = false;
            for (int i = 0; i < hotbar.getSlotCount(); ++i) {
                if (hotbar.getSlot(i) == nullptr) {
                    inv.addItem(item);
                    hotbar.setSlot(i, inv.getItems().back().get());
                    placedInHotbar = true;
                    break;
                }
            }
            if (placedInHotbar) {
                it = worldItems.erase(it);
                continue;
            }
            // No space in hotbar
            inv.addItem(item);
            it = worldItems.erase(it);
            continue;
        }

        // No space in inventory
        ++it;
    }
}

void Game::update(float deltaTime) {
    sf::Vector2f oldPlayerPos = player ? player->getPosition() : sf::Vector2f(0.f, 0.f);

    for (auto& obj : gameObjects) {obj->update(deltaTime, window);}
    for (auto& tree : visibleTrees) {tree->update(deltaTime, window);}
    for (auto& item : worldItems) {item->update(deltaTime, window);}
    for (auto it = saplings.begin(); it != saplings.end(); ) {
        (*it)->update(deltaTime, window);
        if ((*it)->isReadyToBecomeTree()) {
            // Sapling wird zu Baum
            visibleTrees.push_back(std::make_unique<Tree>(
                (*it)->getPosition(),
                resources.getTexture("tree"),resources.getTexture("trunk"),resources.getTexture("wood"),resources.getTexture("sapling"),resources.getTexture("apple")
            ));
            it = saplings.erase(it);
        } else {
            ++it;
        }
    }

    portals.update(deltaTime, window);
    
    if (player) {
        checkItemPickup();
        
        portals.tryTeleport(*player);
        

        //add colision objects to resolve 
        std::vector<GameObject*> blockers;
        for (const auto& tree : visibleTrees) {
            if (!tree->isChopped()) {
                blockers.push_back(tree.get());
            }
        }
        resolveCollisions(oldPlayerPos, blockers);
        camera.setCenter(player->getPosition());
    }
}

void Game::resolveCollisions(const sf::Vector2f& oldPos, const std::vector<GameObject*>& blockers) {
    // Speichere die Zielposition nach Movement
    sf::Vector2f targetPos = player->getPosition();

    // 1. X-Richtung testen (Y bleibt alt)
    player->setPosition(sf::Vector2f(targetPos.x, oldPos.y));
    bool collisionX = false;
    for (const auto* blocker : blockers) {
        if (player->getGlobalBounds().intersects(blocker->getGlobalBounds())) {
            collisionX = true;
            break;
        }
    }
    float finalX = collisionX ? oldPos.x : targetPos.x;

    // 2. Y-Richtung testen (X ist jetzt finalX, Y ist die neue Zielposition!)
    player->setPosition(sf::Vector2f(finalX, targetPos.y));
    bool collisionY = false;
    for (const auto* blocker : blockers) {
        if (player->getGlobalBounds().intersects(blocker->getGlobalBounds())) {
            collisionY = true;
            break;
        }
    }
    float finalY = collisionY ? oldPos.y : targetPos.y;

    // Endgültige Position setzen
    player->setPosition(sf::Vector2f(finalX, finalY));
}

void Game::render() {
    window.clear();
    window.setView(camera);

    //draw background repeatedly untill view is filled
    sf::Vector2f camPos = camera.getCenter();
    sf::Vector2u texSize = resources.getTexture("background").getSize();
    sf::Vector2u winSize = window.getSize();

    // round to nearest tile size below
    float startX = std::floor((camPos.x - winSize.x / 2.f) / texSize.x) * texSize.x;
    float startY = std::floor((camPos.y - winSize.y / 2.f) / texSize.y) * texSize.y;

    // number of tiles needed to fill the view
    int tilesX = winSize.x / texSize.x + 2;
    int tilesY = winSize.y / texSize.y + 2;

    sf::Sprite bg(resources.getTexture("background"));

    //draw all background tiles
    for (int i = 0; i < tilesX; ++i) {
        for (int j = 0; j < tilesY; ++j) {
            bg.setPosition(startX + i * texSize.x, startY + j * texSize.y);
            window.draw(bg); 
        }
    }

    portals.draw(window);

    for (auto& obj : gameObjects) {
        obj->draw(window);
        //draw bounds
        if (Player* player = dynamic_cast<Player*>(obj.get())) {//debug
            auto pb = player->getGlobalBounds();                //debug
            sf::RectangleShape r({pb.width, pb.height});        //debug
            r.setPosition(pb.left, pb.top);                     //debug
            r.setFillColor(sf::Color(255, 0, 0, 100));          //debug
            window.draw(r);                                     //debug
        }
    }

    for (auto& item : worldItems) {
        item->draw(window);
        //draw bounds
        auto ib = item->getGlobalBounds();                      //debug
        sf::RectangleShape r({ib.width, ib.height});            //debug
        r.setPosition(ib.left, ib.top);                         //debug
        r.setFillColor(sf::Color(0, 0, 255, 100));              //debug
        window.draw(r);                                         //debug
    }

    for (const auto& tree : visibleTrees) {
        tree->draw(window);
        //draw bounds
        auto tb = tree->getGlobalBounds();                      //debug
        sf::RectangleShape r({tb.width, tb.height});            //debug
        r.setPosition(tb.left, tb.top);                         //debug
        r.setFillColor(sf::Color(0, 255, 0, 100));              //debug
        window.draw(r);
    }

    for (auto& sapling : saplings) sapling->draw(window);

    // draw UI elements like inventory
    window.setView(uiView);

    for (auto& obj : gameObjects) {
        if (auto* btn = dynamic_cast<Button*>(obj.get())) {
            btn->draw(window);
        }
        if (player){
            Inventory& inv = player->getInventory();
            if (inv.isVisible()) {
                inv.draw(window);

                sf::FloatRect invSize = inv.getGlobalBounds();
                int hotbarX = invSize.left + invSize.width / 2 - 25 * 5; // assuming 10 slots, each 50px wide
                int hotbarY = invSize.top + invSize.height + 10; // 10 pixels below the inventory

                player->getHotbar().draw(window, hotbarX, hotbarY);
            } else {
            // hotbar always centered at bottom
            int slotCount = player->getHotbar().getSlotCount();
            int slotWidth = 50;
            int spacing = 8;
            int totalWidth = slotCount * slotWidth + (slotCount - 1) * spacing;
            int hotbarX = window.getSize().x / 2 - totalWidth / 2;
            int hotbarY = window.getSize().y - slotWidth - 50;

            player->getHotbar().draw(window, hotbarX, hotbarY);
            }
        }
    }

    window.display();
}
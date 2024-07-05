

#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ModifyAttemptCountPopup : public geode::Popup<GJGameLevel*> {


    geode::SeedValueRSV* attempts;
    CCTextInputNode* inputNode;
    protected:

        bool boundsCheck(int num){
            if ((*attempts > 0 && num > std::numeric_limits<int>::max() - *attempts) ||
                (*attempts < 0 && num < std::numeric_limits<int>::min() - *attempts)) {
                log::warn("Operation would result in overflow or underflow");
                return false;
            }
            return true;
        }

        void addAttempts(int num){
            if(boundsCheck(num)){
                *attempts = *attempts + num;
            }
        }
        void subtractAttempts(int num){
            if(boundsCheck(num)){
                *attempts = *attempts - num;
            }
        }
        void setAttempts(int num){
            *attempts = num;
        }
        void onSetButtonClick(CCObject* sender){
            if(inputNode->getString().empty()){return;}
            try {
                int val = std::stoi(inputNode->getString());
                setAttempts(val);
                this->removeMeAndCleanup();
            } catch (const std::invalid_argument& e) {
                log::warn("Input is not a valid integer");
            } catch (const std::out_of_range& e) {
                log::warn("Input is out of range");
            }
        }
        void onPlusButtonClick(CCObject* sender){
            if(inputNode->getString().empty()){return;}
            try {
                int val = std::stoi(inputNode->getString());
                std::cout << val << std::endl;
                addAttempts(val);
                this->removeMeAndCleanup();
            } catch (const std::invalid_argument& e) {
                log::warn("Input is not a valid integer");
            } catch (const std::out_of_range& e) {
                log::warn("Input is out of range");
            }
        }
        void onMinusButtonClick(CCObject* sender){
            if(inputNode->getString().empty()){return;}
            try {
                int val = std::stoi(inputNode->getString());
                std::cout << val << std::endl;
                subtractAttempts(val);
                this->removeMeAndCleanup();
            } catch (const std::invalid_argument& e) {
                log::warn("Input is not a valid integer");
            } catch (const std::out_of_range& e) {
                log::warn("Input is out of range");
            }
        }
        bool setup(GJGameLevel* level) override {
            auto winSize = CCDirector::sharedDirector()->getWinSize();
            attempts = &level->m_attempts;

            this->setTitle("Modify Attempt Count");

            auto menu = CCMenu::create();

            auto input = CCTextInputNode::create(100, 100, "Attempts", "bigFont.fnt");
                       input->setScale(0.75);
            input->setPosition(CCPoint(272.5, 170 ));
            input->setAllowedChars("0123456789");
            inputNode = input;

            auto setSprite = ButtonSprite::create("Set");
            setSprite->setScale(0.80);
            auto setButton = CCMenuItemSpriteExtra::create(
                setSprite, this, menu_selector(ModifyAttemptCountPopup::onSetButtonClick)
            );

            auto addSprite = ButtonSprite::create("+");
            addSprite->setScale(0.80);
            auto addButton = CCMenuItemSpriteExtra::create(
                addSprite, this, menu_selector(ModifyAttemptCountPopup::onPlusButtonClick)
            );

            auto removeSprite = ButtonSprite::create("-");
            removeSprite->setScale(0.80);
            auto removeButton = CCMenuItemSpriteExtra::create(
                removeSprite, this, menu_selector(ModifyAttemptCountPopup::onMinusButtonClick)
            );
            
            
            menu->addChildAtPosition(setButton, Anchor::Center, CCPoint(0, -50));
            menu->addChildAtPosition(addButton, Anchor::Center, CCPoint(40, -50));
            menu->addChildAtPosition(removeButton, Anchor::Center, CCPoint(-39, -50));
            menu->updateLayout();
            this->addChild(input);
            this->addChild(menu);

            return true;
        }

    public:
        static ModifyAttemptCountPopup* create(GJGameLevel* level) {
            auto ret = new ModifyAttemptCountPopup();
            if (ret->initAnchored(240.f, 160.f, level)) {
                ret->autorelease();
                return ret;
            }

            delete ret;
            return nullptr;
        }
};

class $modify (AttemptSetterEditLevelLayer, EditLevelLayer){
	void onAttemptSetButtonClick(CCObject* sender){
		auto modifyAttemptCountPopup = ModifyAttemptCountPopup::create(m_level);
		
		modifyAttemptCountPopup->m_scene = this;
        modifyAttemptCountPopup->m_noElasticity = true;
		modifyAttemptCountPopup->show();
	}
	bool init(GJGameLevel* level){
		if(!EditLevelLayer::init(level)) return false;

		auto menu = CCMenu::create();
		std::string input = std::to_string(level->m_attempts);
		auto spr = ButtonSprite::create("Set Attempts");
		spr->setScale(0.25);

		auto folderMenu = getChildByID("folder-menu");
		auto btn = CCMenuItemSpriteExtra::create(
			spr, this, menu_selector(AttemptSetterEditLevelLayer::onAttemptSetButtonClick)
		);

		folderMenu->addChild(btn);
		folderMenu->updateLayout();
		
		return true;
	}
};

class $modify (AttemptSetterLevelInfoLayer, LevelInfoLayer){
	void onAttemptSetButtonClick(CCObject* sender){
		auto modifyAttemptCountPopup = ModifyAttemptCountPopup::create(m_level);
		
		modifyAttemptCountPopup->m_scene = this;
        modifyAttemptCountPopup->m_noElasticity = true;
		modifyAttemptCountPopup->show();
	}
	bool init(GJGameLevel* level, bool challenge){
		if(!LevelInfoLayer::init(level, challenge)) return false;

		auto menu = CCMenu::create();
		std::string input = std::to_string(level->m_attempts);
		auto spr = ButtonSprite::create("Set Attempts");
		spr->setScale(0.25);

		auto leftSideMenu = getChildByID("left-side-menu");
		auto btn = CCMenuItemSpriteExtra::create(
			spr, this, menu_selector(AttemptSetterLevelInfoLayer::onAttemptSetButtonClick)
		);

		leftSideMenu->addChild(btn);
		leftSideMenu->updateLayout();
		
		return true;
	}
};






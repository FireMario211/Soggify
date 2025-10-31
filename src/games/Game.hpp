#pragma once
class Game : public geode::Popup<> {
protected:
    std::string m_name;
    bool initGame(std::string name);
public:
    const std::string& getName() const { return m_name; }
    virtual void onComplete() {};
};

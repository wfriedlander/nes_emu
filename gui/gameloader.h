#ifndef GAMELOADER_H
#define GAMELOADER_H

#include <QStringList>

#include "core/nes.h"


class GameLoader
{
public:
    GameLoader(Core::NES* nes);

public:
    bool LoadGame(const QString& filename);
    const QStringList& RecentGames();

private:
    Core::NES* mNes = nullptr;
    QStringList mRecent;
};

#endif // GAMELOADER_H

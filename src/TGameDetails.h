#ifndef TGAMEDETAILS_H
#define TGAMEDETAILS_H
/***************************************************************************
 *   Copyright (C) 2022 by Stephen Lyons - slysven@virginmedia.com         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "utils.h" // For qsl(...)

#include <QString>
#include <QList>
#include <QStringList>

struct GameDetail
{
    QString name;
    QString hostUrl;
    int port = 0;
    bool tlsEnabled = false;
    QString websiteInfo;
    QString icon;
    QString description;
    // how the game installs its own full interface, if it does - the generic
    // starter UI is then not preinstalled for it:
    enum class OwnUi {
        None,
        // a loader bundled with Mudlet fetches the interface itself
        BundledLoader,
        // the game sends a Client.GUI package, often only after login
        ClientGui
    };
    OwnUi ownUi = OwnUi::None;
    // other hostnames the game is reachable under:
    QStringList alternateHostUrls;
};

class TGameDetails
{
public:
    inline static const QList<GameDetail>::const_iterator findGame(const QString& key, const Qt::CaseSensitivity sensitivity = Qt::CaseSensitive)
    {
        QList<GameDetail>::const_iterator i;
        for (i = scmDefaultGames.constBegin(); i != scmDefaultGames.constEnd(); ++i) {
            if (!(*i).name.compare(key, sensitivity)) {
                return i;
            }
        }
        // Return the past-the-end iterator if not found:
        return i;
    }

    inline static const QStringList keys()
    {
        QStringList result;
        for (auto i = scmDefaultGames.constBegin(); i != scmDefaultGames.constEnd(); ++i) {
            result << (*i).name;
        }
        return result;
    }

    inline static GameDetail::OwnUi gameOwnUi(const QString& hostUrl)
    {
        for (const auto& game : scmDefaultGames) {
            if (game.ownUi != GameDetail::OwnUi::None && (!game.hostUrl.compare(hostUrl, Qt::CaseInsensitive) || game.alternateHostUrls.contains(hostUrl, Qt::CaseInsensitive))) {
                return game.ownUi;
            }
        }
        return GameDetail::OwnUi::None;
    }

    // clang-format off
    // AvatarMUD build: the only game offered in the connection dialog
    inline static const QList<GameDetail> scmDefaultGames = {
            {qsl("AvatarMUD"),
             qsl("avatar.outland.org"),
             3000,
             false,
             qsl("avatar.outland.org port 3000"),
             qsl(":/icons/avatarmud_480x120.png"),
             qsl("Welcome to AvatarMUD! Select this profile, enter your character name and "
                 "password (or leave them blank to create a new character), and press Connect."),
             // the bundled AvatarUI package is the game's interface, so no starter UI:
             GameDetail::OwnUi::BundledLoader},

            // Deliberately without artwork: the connection dialog draws an entry only as its artwork, so this
            // testing aid (it needs Busted, which players lack) is listed and selectable yet invisible, while a
            // developer can reach it with --profile="Mudlet self-test" or from "My games" in a debug build.
            // See https://github.com/Mudlet/Mudlet/issues/6443
            {qsl("Mudlet self-test"),
             qsl("mudlet.org"),
             23,
             false,
             qsl("<a href='https://www.mudlet.org'>www.mudlet.org"),
             QString(), // empty on purpose - see the note above
             qsl("This isn't a game profile, but a special one for testing Mudlet itself using "
                 "Busted. You can also use it as a starting point to create automated tests for your "
                 "own profiles!")},
            };
    // clang-format on
};

#endif // TGAMEDETAILS_H

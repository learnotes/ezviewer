/****************************************************************************
 * EZ Viewer
 * Copyright (C) 2013 huangezhao. CHINA.
 * Contact: huangezhao (huangezhao@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ***************************************************************************/

#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QMap>
#include <QStringList>

#include "action.h"


class ActionManager
{
public:
    template <typename PT, typename T, typename ReturnType>
    static void registerFunction(const QString &description, PT *obj,
                          ReturnType (T::*function)(), const QString &script);

    template <typename PT, typename T, typename ReturnType, typename ArgumentType>
    static void registerFunction(const QString &description,
                                 PT *obj,
                                 ReturnType (T::*function)(ArgumentType),
                                 const QString &script,
                                 ArgumentType arg);

    static bool bindShortcut(const QString &keySequence, const QString &actionScript);
    static void bindShortcut(const QStringList &keySequences, const QString &actionScript);
    static bool unbindShortcut(const QString &keySequence);
    static void unbindShortcut(const QStringList &keySequences);

    static void unbindAllShortcut() { shortcutMap.clear(); }

    // return: action description list, and if actionScript is not NULL, the action script list will be set.
    static QStringList getAllActions(QStringList *actionScript = NULL);
    // return: action script text that this key sequence bind with.
    static QString getMatchAction(const QString &keySequence);
    // return: all key sequence list that bind with the action script.
    static QStringList getBindShortcuts(const QString &actionScript);
    // return: all key sequence and action script pair.
    static QMap<QString, QString> getAllShortcut() { return shortcutMap; }

    // run the function that bind with the key sequence.
    static bool run(const QString &keySequence);

private:
    ~ActionManager();

    static QMap<QString, Action*> actionMap;   // actionScript - action
    static QMap<QString, QString> shortcutMap; // keySequence  - actionScript

    static ActionManager ensure_destructor_invoke_while_quit;
};



template <typename PT, typename T, typename ReturnType>
inline void ActionManager::registerFunction(const QString &description, PT *obj,
                      ReturnType (T::*function)(), const QString &script)
{
    actionMap.insert(script,
                     new ActionImpl<T, ReturnType>(description, obj, function) );
}

template <typename PT, typename T, typename ReturnType, typename ArgumentType>
inline void ActionManager::registerFunction(const QString &description,
                                            PT *obj,
                                            ReturnType (T::*function)(ArgumentType),
                                            const QString &script,
                                            ArgumentType arg)
{
    actionMap.insert(script,
                     new ActionImplWithArgument<T, ReturnType, ArgumentType>
                     (description, obj, function, arg) );
}

inline QString ActionManager::getMatchAction(const QString &keySequence)
{
    return shortcutMap.contains(keySequence) ? shortcutMap.value(keySequence)
                                             : QString::null;
}


#endif // ACTIONMANAGER_H

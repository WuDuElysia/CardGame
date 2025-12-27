#ifndef UNDO_MANAGER_H
#define UNDO_MANAGER_H

#include <vector>
#include "models/GameModel.h"
#include "models/UndoModel.h"

class UndoManager {
public:
    UndoManager();
    virtual ~UndoManager();
    
    bool init(GameModel* gameModel);
    void recordUndoAction(const UndoModel& undoAction);
    bool undo();
    bool canUndo() const;
    int getUndoCount() const;
    void clearUndoHistory();
    void setMaxUndoSteps(int maxSteps);
    int getMaxUndoSteps() const;

private:
    GameModel* _gameModel;
    std::vector<UndoModel> _undoHistory;
    int _maxUndoSteps;
};

#endif // __UNDO_MANAGER_H__

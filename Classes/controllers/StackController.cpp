#include "StackController.h"
#include "models/UndoModel.h"

StackController::StackController(GameModel* gameModel, StackView* stackView)
	: _gameModel(gameModel)
	, _stackView(stackView)
	, _undoManager(nullptr)
	, _cardStackManager(nullptr) {
}

StackController::~StackController() {
	if (_cardStackManager) {
		delete _cardStackManager;
		_cardStackManager = nullptr;
	}
}

bool StackController::init() {
	// 初始化牌堆管理器
	_cardStackManager = new CardStackManager();
	if (!_cardStackManager->init(_gameModel)) {
		delete _cardStackManager;
		_cardStackManager = nullptr;
		return false;
	}

	// 注册牌堆点击回调
	if (_stackView) {
		_stackView->setStackClickCallback([this]() {
			this->handleStackClick();
			});

		// 注册底牌点击回调
		_stackView->setTrayClickCallback([this](int cardId) {
			this->handleTrayClick(cardId);
			});

		// 先清空现有的牌堆卡牌
		_stackView->clearAllStackCards();
		
		// 根据GameModel的最新数据重新创建牌堆卡牌
		const auto& stackCards = _gameModel->getStackCards();
		for (const auto& cardModel : stackCards) {
			auto cardView = CardView::create(cardModel.get());
			if (cardView) {
				cardView->setFlipped(true);

				// 添加到牌堆，默认打开
				_stackView->addStackCard(cardView);
			}
		}

		// 根据GameModel的最新数据重新创建底牌
		CardModel* trayCard = _gameModel->getTrayCard();
		if (trayCard) {
			auto cardView = CardView::create(trayCard);
			if (cardView) {
				// 将卡牌进行默认打开
				cardView->setFlipped(true);

				// 设置底牌
				_stackView->setTrayCard(cardView);
			}
		}
	}

	return true;
}

// 新增方法：重新初始化UI，用于撤销操作后更新UI
void StackController::reinitUI() {
	// 先销毁现有的StackController
	if (_cardStackManager) {
		delete _cardStackManager;
		_cardStackManager = nullptr;
	}

	// 重新初始化
	init();
}

bool StackController::handleStackClick() {
	// 记录撤销操作
	if (_undoManager) {
		UndoModel undoAction;
		undoAction.setActionType("replaceTray");

		// 记录当前底牌信息（初始状态下底牌可能为空）
		CardModel* currentTrayCard = _cardStackManager->getCurrentTrayCard();
		if (currentTrayCard) {
			undoAction.setCardId(currentTrayCard->getCardId());
		} else {
			// 初始状态下底牌为空，设置特殊标识
			undoAction.setCardId(-1); // 表示空底牌
		}

		// 记录当前牌堆数量
		undoAction.setOldStackCardCount(_cardStackManager->getStackCardCount());

		_undoManager->recordUndoAction(undoAction);
	}

	if (!_stackView || _stackView->getStackCardCount() == 0) {
		return false;
	}

	// 从备用牌堆移除顶部卡牌视图（只从向量中移除，保留在父节点中用于动画）
	CardView* topStackCard = _stackView->removeTopStackCard();
	if (!topStackCard) {
		return false;
	}

	// 保存卡牌的当前位置（备用牌堆顶部位置）
	cocos2d::Vec2 startPos = topStackCard->getPosition();

	// 从备用牌堆抽取卡牌替换底牌（更新模型）
	bool result = replaceTrayWithStackCard();
	
	if (_stackView && result) {
		// 获取底牌位置
		cocos2d::Vec2 trayPos = _stackView->getTrayPosition();

		// 执行卡牌从备用牌堆到底牌堆的移动动画
		_stackView->moveCardToTargetWithAnimation(topStackCard, trayPos, 0.5f, [this, topStackCard]() {
			// 动画完成后，将卡牌设置为新的底牌
			if (_stackView && topStackCard) {
				// 从原父节点移除卡牌（如果还在的话）
				if (topStackCard->getParent()) {
					topStackCard->removeFromParent();
				}
				
				// 设置新底牌
				_stackView->setTrayCard(topStackCard);
				// 播放抽取动画效果
				_stackView->playDrawCardAnimation();
				// 更新牌堆显示
				_stackView->updateStackDisplay();
				// 释放之前手动增加的引用计数
				topStackCard->release();
			}
		});
	}
	
	return result;
}

bool StackController::handleTrayClick(int cardId) {
	// 搴曠墝鐐瑰嚮澶勭悊
	// 鍙互鏍规嵁闇€瑕佹坊鍔犵浉鍏抽€昏緫
	return true;
}

bool StackController::replaceTrayWithStackCard() {
	if (!_cardStackManager) {
		return false;
	}

	return _cardStackManager->replaceTrayFromStack();
}

bool StackController::isStackEmpty() const {
	if (!_cardStackManager) {
		return true;
	}

	return _cardStackManager->isStackEmpty();
}

void StackController::setUndoManager(UndoManager* undoManager) {
	_undoManager = undoManager;
}




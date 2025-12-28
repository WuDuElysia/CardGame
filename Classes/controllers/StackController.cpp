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
	// 璁板綍鎾ら攢鎿嶄綔
	if (_undoManager) {
		UndoModel undoAction;
		undoAction.setActionType("replaceTray");

		// 璁板綍褰撳墠搴曠墝淇℃伅（初始状态下底牌可能为空）
		CardModel* currentTrayCard = _cardStackManager->getCurrentTrayCard();
		if (currentTrayCard) {
			undoAction.setCardId(currentTrayCard->getCardId());
		} else {
			// 初始状态下底牌为空，设置特殊标识
			undoAction.setCardId(-1); // 表示空底牌
		}

		// 璁板綍褰撳墠鐗屽爢鏁伴噺
		undoAction.setOldStackCardCount(_cardStackManager->getStackCardCount());

		_undoManager->recordUndoAction(undoAction);
	}

	// 从备用牌堆抽取卡牌替换底牌
	bool result = replaceTrayWithStackCard();
	
	// 更新视图
	if (_stackView && result) {
		// 直接更新视图，禁用动画以避免同步问题
		_stackView->playDrawCardAnimation();
		_stackView->updateStackDisplay();
		
		// 关键修复：创建新的底牌视图并设置到StackView
		CardModel* newTrayCard = _gameModel->getTrayCard();
		if (newTrayCard) {
			// 创建新的底牌视图
			auto newTrayCardView = CardView::create(newTrayCard);
			if (newTrayCardView) {
				// 设置底牌到视图
				_stackView->setTrayCard(newTrayCardView);
			}
		}
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




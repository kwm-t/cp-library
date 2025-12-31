#include <set>
#include <iostream>
#include <functional>

template<typename T>
class RangeSet {
public:
	// --- コンストラクタ ---
	explicit RangeSet(T limit)
		: TINF(limit + 1)
	{
		ranges_.emplace(-TINF, -TINF);
		ranges_.emplace(TINF, TINF);
	}

	// --- 公開API ---

	// 区間がすでにカバーされているか
	bool isCovered(T l, T r) const noexcept {
		auto it = std::prev(ranges_.lower_bound({ l + 1, l + 1 }));
		return (it->first <= l && r <= it->second);
	}
	bool isCovered(T x) const noexcept { return isCovered(x, x); }

	// カバーしている区間を返す（存在しなければ {-TINF, -TINF}）
	std::pair<T, T> getCoveringRange(T l, T r) const noexcept {
		auto it = std::prev(ranges_.lower_bound({ l + 1, l + 1 }));
		if (it->first <= l && r <= it->second) return *it;
		return { -TINF, -TINF };
	}
	std::pair<T, T> getCoveringRange(T x) const noexcept { return getCoveringRange(x, x); }

	// 先頭区間を取得
	std::pair<T, T> front() const noexcept {
		auto it = ranges_.begin();
		++it;
		return *it;
	}

	// 末尾区間を取得
	std::pair<T, T> back() const noexcept {
		auto it = ranges_.end();
		--it; --it;
		return *it;
	}

	// 区間挿入（マージあり）
	void insertRange(
		T l, T r,
		const std::function<void(T, T)>& onInsert = [](T, T) {},
		const std::function<void(T, T)>& onErase = [](T, T) {}
	) {
		auto it = std::prev(ranges_.lower_bound({ l + 1, l + 1 }));

		if (isCovered(l, r)) return; // 既に含まれている

		// 左端が既存区間に隣接 or 内包されている場合はマージ
		if (it->first <= l && l <= it->second + 1) {
			l = it->first;
			eraseRange(it, onErase);
		}
		else {
			++it;
		}

		// 右端方向にマージ
		while (r > it->second) eraseRange(it, onErase);

		if (it->first - 1 <= r && r <= it->second) {
			r = it->second;
			eraseRange(it, onErase);
		}

		addRange(l, r, onInsert);
	}

	// 1点挿入
	void insertPoint(
		T x,
		const std::function<void(T, T)>& onInsert = [](T, T) {},
		const std::function<void(T, T)>& onErase = [](T, T) {}
	) {
		insertRange(x, x, onInsert, onErase);
	}

	// 区間削除
	void eraseRange(
		T l, T r,
		const std::function<void(T, T)>& onInsert = [](T, T) {},
		const std::function<void(T, T)>& onErase = [](T, T) {}
	) {
		auto it = std::prev(ranges_.lower_bound({ l + 1, l + 1 }));

		// カバーされている1区間のみのケース
		if (it->first <= l && r <= it->second) {
			onErase(it->first, it->second);
			auto old = *it;
			ranges_.erase(it);
			if (old.first < l) addRange(old.first, l - 1, onInsert);
			if (r < old.second) addRange(r + 1, old.second, onInsert);
			return;
		}

		// 左端処理
		if (it->first <= l && l <= it->second) {
			if (it->first < l) addRange(it->first, l - 1, onInsert);
			eraseRange(it, onErase);
		}
		else {
			++it;
		}

		// 中央部削除
		while (it->second <= r) eraseRange(it, onErase);

		// 右端処理
		if (it->first <= r && r <= it->second) {
			if (r < it->second) addRange(r + 1, it->second, onInsert);
			eraseRange(it, onErase);
		}
	}

	// 1点削除
	void erasePoint(
		T x,
		const std::function<void(int, int)>& onInsert = [](int, int) {},
		const std::function<void(int, int)>& onErase = [](int, int) {}
	) {
		eraseRange(x, x, onInsert, onErase);
	}

	// 登録区間数
	int rangeCount() const noexcept { return static_cast<int>(ranges_.size()) - 2; }

	// Mex（未カバー最小値）
	T mex(T x = 0) const noexcept {
		if (!isCovered(x)) return x;
		auto it = std::prev(ranges_.lower_bound({ x + 1, x + 1 }));
		return it->second + 1;
	}

	// デバッグ表示
	void debug() const noexcept {
		for (auto[l, r] : ranges_)
			if (std::abs(l) != TINF)
				std::cout << l << " " << r << "\n";
	}

private:
	// --- 内部状態 ---
	std::set<std::pair<T, T>> ranges_;
	T TINF;

	// --- 内部ヘルパー ---
	void eraseRange(typename std::set<std::pair<T, T>>::iterator& it,
		const std::function<void(int, int)>& onErase) {
		onErase(it->first, it->second);
		it = ranges_.erase(it);
	}

	void addRange(T l, T r,
		const std::function<void(int, int)>& onInsert) {
		onInsert(l, r);
		ranges_.emplace(l, r);
	}
};
// 区間の変更があるとコールバックが呼ばれるのでセグ木で良い感じにしたい場合とかはいい感じにする
// https://atcoder.jp/contests/abc430/submissions/70631649


// 半開区間ver
// 実績なし
#include <set>
#include <iostream>
#include <functional>

template <typename T>
class RangeSet {
public:
    using Range = std::pair<T, T>;

    explicit RangeSet(T lim) : TINF_(lim + 1) {
        ranges_.emplace(-TINF_, -TINF_);
        ranges_.emplace(TINF_, TINF_);
    }

    // 区間 [l, r) を追加
    void insertRange(
        T l, T r,
        const std::function<void(T, T)> &onInsert = [](T, T) {},
        const std::function<void(T, T)> &onErase = [](T, T) {}
    ) {
        if (l >= r) return;

        auto eraseRange = [&](auto &it) {
            onErase(it->first, it->second);
            it = ranges_.erase(it);
        };

        auto addRange = [&](T L, T R) {
            onInsert(L, R);
            ranges_.emplace(L, R);
        };

        auto it = std::prev(ranges_.lower_bound({ l + 1, l + 1 }));
        if (isCovered(l, r)) return;

        // 左端と連結
        if (it->first <= l && l <= it->second) {
            l = it->first;
            eraseRange(it);
        } else {
            ++it;
        }

        // 中央の連結
        while (it->first <= r) eraseRange(it);

        // 右端の連結
        auto nxt = ranges_.lower_bound({ r, r });
        if (nxt->first <= r && r < nxt->second) {
            r = nxt->second;
            eraseRange(nxt);
        }

        addRange(l, r);
    }

    void insertPoint(
        T x,
        const std::function<void(T, T)> &onInsert = [](T, T) {},
        const std::function<void(T, T)> &onErase = [](T, T) {}
    ) {
        insertRange(x, x + 1, onInsert, onErase);
    }

    // 区間 [l, r) を削除
    void eraseRange(
        T l, T r,
        const std::function<void(T, T)> &onInsert = [](T, T) {},
        const std::function<void(T, T)> &onErase = [](T, T) {}
    ) {
        if (l >= r) return;

        auto eraseIt = [&](auto &it) {
            onErase(it->first, it->second);
            it = ranges_.erase(it);
        };

        auto addRange = [&](T L, T R) {
            if (L < R) {
                onInsert(L, R);
                ranges_.emplace(L, R);
            }
        };

        auto it = std::prev(ranges_.lower_bound({ l + 1, l + 1 }));

        // 1つの範囲内にすべて含まれるケース
        if (it->first <= l && r <= it->second) {
            onErase(it->first, it->second);
            auto old = *it;
            ranges_.erase(it);
            addRange(old.first, l);
            addRange(r, old.second);
            return;
        }

        // 左端部分削除
        if (it->first <= l && l < it->second) {
            addRange(it->first, l);
            eraseIt(it);
        } else {
            ++it;
        }

        // 中央削除
        while (it->first < r && it->second <= r) eraseIt(it);

        // 右端部分
        if (it->first < r && r < it->second) {
            addRange(r, it->second);
            eraseIt(it);
        }
    }

    void erasePoint(
        T x,
        const std::function<void(T, T)> &onInsert = [](T, T) {},
        const std::function<void(T, T)> &onErase = [](T, T) {}
    ) {
        eraseRange(x, x + 1, onInsert, onErase);
    }

    // mex: 最小の非被覆点を返す
    T mex(T x = 0) const {
        if (!isCovered(x, x + 1)) return x;
        auto it = std::prev(ranges_.lower_bound({ x + 1, x + 1 }));
        return it->second;
    }

    // 区間数（番兵を除く）
    int size() const { return static_cast<int>(ranges_.size()) - 2; }

    // デバッグ出力
    void debug() const {
        for (auto [l, r] : ranges_) {
            if (abs(l) == TINF_) continue;
            std::cout << "[" << l << ", " << r << ")\n";
        }
    }

private:
    std::set<Range> ranges_;
    T TINF_;

    bool isCovered(T l, T r) const {
        auto it = std::prev(ranges_.lower_bound({ l + 1, l + 1 }));
        return it->first <= l && r <= it->second;
    }
};

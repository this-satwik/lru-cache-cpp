cout << "=== LRU Cache Phase 3 Test ===" << endl;
    LRUCache cache(3);

    cache.set("A", "10");
    cache.set("B", "20");
    cache.set("C", "30");

    cout << "GET A: " << cache.get("A") << " (expected: 10)" << endl;

    cache.set("D", "40"); // should evict B (LRU after GET A moved A to front)

    cout << "GET B: " << cache.get("B") << " (expected: MISS)" << endl;
    cout << "GET D: " << cache.get("D") << " (expected: 40)" << endl;
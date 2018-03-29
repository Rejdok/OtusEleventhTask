
#include "CommandProcessor.h"
#include "async.h"
#include <map>
#include <set>
std::map<int, async::handle_t> handlers;
int p = 0;
std::vector<std::string> commands{"1", "1\n","\n2\n3\n4\n5\n6\n{\na\n","b\nc\nd\n}\n89\n" };

void c() {
	handlers.emplace(p,async::connect(3));
	p++;
}
void b() {
	for (auto& i : handlers) {
		async::disconnect(i.second);
	}
}
void a(int handleNum) {
	for (auto& i : commands) {
		async::receive(handlers[handleNum], i.c_str(), i.size());
	}
}

int main(int, char *[]) {
	std::size_t bulk = 5;
	handlers.emplace(p,async::connect(bulk));
	p++;
	handlers.emplace(p,async::connect(bulk-2));
	p++;
	std::thread ab(a,0);
	std::thread bb(a,1);
	std::thread cb(c);
	ab.join();
	bb.join();
	cb.join();
	std::thread ff(b);
	ff.join();
	return 0;
}

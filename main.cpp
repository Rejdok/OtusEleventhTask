
#include "CommandProcessor.h"
#include "async.h"


std::vector<async::handle_t> handlers;
std::vector<std::string> commands{"1", "1\n","\n2\n3\n4\n5\n6\n{\na\n","b\nc\nd\n}\n89\n" };

void c() {
	handlers.emplace_back(async::connect(3));
}
void b() {
	for (auto& i : handlers) {
		async::disconnect(i);
	}
}
void a(async::handle_t handle) {
	for (auto& i : commands) {
		async::receive(handlers[handle], i.c_str(), i.size());
	}
}

int main(int, char *[]) {
	std::size_t bulk = 5;
	handlers.emplace_back(async::connect(bulk));
	handlers.emplace_back(async::connect(bulk));
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

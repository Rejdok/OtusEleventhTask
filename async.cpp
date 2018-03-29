#include "CommandProcessor.h"

#include <map>
#include <queue>
#include <string>
#include <sstream>
#include <memory>
#include <mutex>

#include "async.h"
namespace async{
	class asyncHandler {
	public:
		asyncHandler() {
			coutMutex = std::make_shared<std::mutex>();
		}
		handle_t registerNewHandler(std::size_t bulk) {
			size_t countOfOutputThreads = 2;
			CommandsProcessor* p= new CommandsProcessor(countOfOutputThreads, bulk, coutMutex);
			registredHandles.emplace(reinterpret_cast<handle_t>(p),p);
			return reinterpret_cast<handle_t>(p);
		}
		void inputNewCommand(handle_t handle,std::string& rawCommand) {
			if (registredHandles.find(handle) != registredHandles.end()) {
				registredHandles[handle]->inputNewCommand(rawCommand);
			}
		}
		void disconnectHandler(handle_t handle) {
			if (registredHandles.find(handle) != registredHandles.end()) {
				registredHandles[handle]->diconnect();
				delete registredHandles[handle];
				registredHandles.erase(handle);
			}
		}
		~asyncHandler() {
			for (auto &i : registredHandles) {
				registredHandles[i.second]->diconnect();
				delete registredHandles[i.second];
				registredHandles.erase(i.first);
			}
		}
	private:	
		size_t maxConnections = maxConnections;
		std::shared_ptr<std::mutex> coutMutex;
		std::map<handle_t, CommandsProcessor*> registredHandles;
		std::map<handle_t, std::queue<std::string>> inputMessagesQueue;
		std::thread queueHandle;
	};
	
	asyncHandler OneObjectToRuleThemAll;

	handle_t connect(std::size_t bulk) {
		return OneObjectToRuleThemAll.registerNewHandler(bulk);
	}

	void receive(handle_t handle, const char *data, std::size_t size) {
		std::istringstream ss(data);
		std::string command;
		while (std::getline(ss, command))
			OneObjectToRuleThemAll.inputNewCommand(handle, command);
	}

	void receive(handle_t handle, std::string &command) {
		OneObjectToRuleThemAll.inputNewCommand(handle, command);
	}

	void disconnect(handle_t handle) {
		OneObjectToRuleThemAll.disconnectHandler(handle);
	}

}

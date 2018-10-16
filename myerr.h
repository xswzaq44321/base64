#include <string>

namespace myerr{
	class wrong_argument{
		public:
			wrong_argument(const std::string& what_arg):
				what(what_arg)
			{
			}
			std::string what;
	};

	class argument_syntax_error{
		public:
			argument_syntax_error()
			{
			}
			argument_syntax_error(const std::string& what_arg):
				what(what_arg)
			{
			}
			std::string what;
	};
}

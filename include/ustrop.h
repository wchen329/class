/* Micro String Operations
 * wchen329
 */
#include "priscas_global.h"
#include "primitives.h"
#include <algorithm>

namespace priscas
{
	class StrOp
	{
		public:

		static bool is_whitespace(char c)
		{
			return c == '\n' || c == '\r' || c == ' ' || c == '\t';
		}

		/* whitespace return
		 * Remove all leading whitespaces 
		 */
		static UPString ws_return(const UPString&);

		/* strip comment
		 * Remove commented code
		 */
		static UPString strip_comment(const UPString&);

		/* tokas
		 * Tokenize a string value assignment pair.
		 */
		static NameValueStringPair tokas(const UPString&);

		/* has_prefix
		 * Check for a specific prefix (sprfx).
		 * If non-matching prefix, return null string
		 * else, return string without the prefix.
		 */
		static UPString has_prefix(const UPString& strin, const UPString& sprfx);

	};
}

#include "ustrop.h"

namespace priscas
{
	// These can be simplified by using
	// C++ std::string member functions 
	// TODO: do that
	UPString StrOp::ws_return(const UPString& ups)
	{
		UPString ret;
		bool notws = false;
		for(char c : ups)
		{
			if(!is_whitespace(c))
			{
				notws = true;
			}

			if(notws)
			{
				ret += c;
			}
		}

		return ret;
	}

	UPString StrOp::strip_comment(const UPString& ups)
	{
		UPString ret;

		for(char c : ups)
		{
			if(c == ';') break;
			if(c == '\n') break;
			ret += c;
		}

		return ret;
	}

	NameValueStringPair StrOp::tokas(const UPString& ups)
	{
		UPString name, value;
		size_t ind = ups.find_first_of("=");

		if(ind == -1)  return NameValueStringPair("", "");

		for(size_t wh = 0; wh < ind; ++wh)
		{
			name += ups[wh];
		}
		for(size_t wh = ind + 1; wh < ups.size(); ++wh)
		{
			value += ups[wh];
		}
		
		return NameValueStringPair(name, value);
	}


	UPString StrOp::has_prefix(const UPString& strin, const UPString& sprfx)
	{
		UPString ret;

		if(sprfx.size() >= strin.size())
		{
			return ret;
		}

		for(size_t ind = 0; ind < sprfx.size(); ++ind)
		{
			if(strin[ind] != sprfx[ind])
			{
				return ret;
			}
		}

		return strin.substr(sprfx.size(), strin.size());

	}
}

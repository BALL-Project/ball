CHECK_CXX_SOURCE_COMPILES(
	"template <class T>
	void self_assign(T& t) noexcept(noexcept(t = t))
	{
	    t = t;
	}
	int main(int, char**)
	{
		int x = 1;
		self_assign(x);
		return 0;
	}" BALL_HAS_NOEXCEPT
)

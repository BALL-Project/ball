CHECK_CXX_SOURCE_COMPILES(
	"int main() { thread_local int i = 0; return 0; }"
	BALL_HAS_THREAD_LOCAL
)

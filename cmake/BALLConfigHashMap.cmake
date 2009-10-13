## Try a number of different ways to detect a suitable hash map implementation
## and if all fails, fall back to map instead
##
## Note: since by now, boost is a required dependency, we might skip all hash_map
##       and map - related tests at some point

INCLUDE(CheckIncludeFileCXX)

MESSAGE(STATUS "Checking for hash map")

## First, try to find an unordered_map implementation
#  For now, we have to disable <unordered_map> without prefix to avoid conflicts with boost
#  CHECK_INCLUDE_FILE_CXX("unordered_map" BALL_HAS_STD_UNORDERED_MAP ${COMPILE_DEFINITIONS})
IF(BALL_HAS_STD_UNORDERED_MAP)
	MESSAGE(STATUS "Found as <unordered_map>!")
	SET(BALL_MAP_NAMESPACE "std")
	SET(BALL_MAP_NAME "std::unordered_map<Key,T>")
	SET(BALL_EXTEND_HASH_IN_STD_NS TRUE)
	SET(BALL_HAS_UNORDERED_MAP TRUE)
ELSE()
	CHECK_INCLUDE_FILE_CXX("tr1/unordered_map" BALL_HAS_TR1_UNORDERED_MAP ${COMPILE_DEFINITIONS})
	IF (BALL_HAS_TR1_UNORDERED_MAP)
		MESSAGE(STATUS "Found as <tr1/unordered_map>!")
		SET(BALL_MAP_NAMESPACE "tr1")
		SET(BALL_MAP_NAME "std::tr1::unordered_map<Key,T>")
		SET(BALL_EXTEND_HASH_IN_STD_NS TRUE)
		SET(BALL_HAS_UNORDERED_MAP TRUE)
	ELSE()
		CHECK_INCLUDE_FILE_CXX("boost/unordered_map" BALL_HAS_BOOST_UNORDERED_MAP ${COMPILE_DEFINITIONS})
		IF (BALL_HAS_BOOST_UNORDERED_MAP)
			MESSAGE(STATUS "Found as <boost/unordered_map>!")
			SET(BALL_MAP_NAMESPACE "boost")
			SET(BALL_MAP_NAME "boost::unordered_map<Key,T>")
			SET(BALL_EXTEND_HASH_IN_STD_NS FALSE)
			SET(BALL_HAS_UNORDERED_MAP TRUE)
		ENDIF()
	ENDIF()
ENDIF()

IF (NOT BALL_HAS_UNORDERED_MAP)
	SET(EXT_INCLUDE_PREFIX_FOUND FALSE)
	FOREACH(TMP_EXT_INCLUDE_PREFIX "" "ext/")
		IF (NOT EXT_INCLUDE_PREFIX_FOUND)
			CHECK_INCLUDE_FILE_CXX(${TMP_EXT_INCLUDE_PREFIX}hash_map EXT_INCLUDE_PREFIX_FOUND ${COMPILE_DEFINITIONS})
			IF (EXT_INCLUDE_PREFIX_FOUND)
				MESSAGE(STATUS "Found as <"${TMP_EXT_INCLUDE_PREFIX}"hash_map>")
				IF (NOT TMP_EXT_INCLUDE_PREFIX STREQUAL "")
					SET(BALL_EXT_INCLUDE_PREFIX TMP_EXT_INCLUDE_PREFIX)
				ENDIF()
			ENDIF()
		ENDIF()
	ENDFOREACH()

	## Find the correct namespace to use
	SET(EXT_NAMESPACE_FOUND FALSE)
	FOREACH(TMP_EXT_NAMESPACE "std" "stdext" "__gnu_cxx")
		IF (NOT EXT_NAMESPACE_FOUND)
			CHECK_CXX_SOURCE_COMPILES("#include <${BALL_EXT_INCLUDE_PREFIX}hash_map>
				int main(int /*argc*/, char** /*argv*/)
				{
					${TMP_EXT_NAMESPACE}::hash_map<int, int> a;

					return 0;
				}"
			EXT_NAMESPACE_FOUND)
			IF (EXT_NAMESPACE_FOUND)
				SET(BALL_MAP_NAMESPACE ${TMP_EXT_NAMESPACE})
				SET(BALL_MAP_NAME ${TMP_EXT_NAMESPACE}"::hash_map<Key, T>")
				SET(BALL_HAS_HASH_MAP)
			ENDIF()
		ENDIF()
	ENDFOREACH()

	IF (NOT EXT_INCLUDE_PREFIX_FOUND OR NOT EXT_NAMESPACE_FOUND)
		MESSAGE(STATUS "No suitable hash_map implementation found! Falling back to std::map!")
		SET(BALL_MAP_NAMESPACE "std")
		SET(BALL_MAP_NAME "std::map<Key, T>")
	ELSE()
		MESSAGE(Check whether LongSize hash is required)
		CHECK_CXX_SOURCE_COMPILES("#include <${BALL_EXT_INCLUDE_PREFIX}hash_map>
			namespace ${TMP_EXT_NAMESPACE} {
				template<>
				struct hash<${BALL_ULONG64_TYPE}>
				{
					size_t operator()(${BALL_ULONG64_TYPE} x) const { return (size_t)x; }
				}

				int main(int /*argc*/, char** /*argv*/)
				{
					return 0;
				}
			}"
			BALL_NEEDS_LONGSIZE_HASH)
	ENDIF()
ENDIF()

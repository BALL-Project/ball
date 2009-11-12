dnl
dnl	Check for hash_map instead of map to speed up things
dnl
AC_DEFUN([CF_CHECK_MAP], [
	AC_MSG_CHECKING(for hash map)
	if test "${HAS_GPLUSPLUS}" = true -a "${USE_TR1}" = true ; then
		AC_MSG_CHECKING(...trying tr1::unordered_map)
		AC_TRY_COMPILE(
		[
		#include <tr1/unordered_map>
		],
		[
		],
		PROJECT[]_HAS_UNORDERED_MAP=true
		)	
		AC_MSG_RESULT(found)
		AC_DEFINE(PROJECT[]_MAP_NAMESPACE, tr1)	
		AC_DEFINE(PROJECT[]_HAS_TR1_UNORDERED_MAP, true)
		AC_SUBST(PROJECT[]_HAS_TR1_UNORDERED_MAP)
		AC_DEFINE(PROJECT[]_MAP_NAME, [std::tr1::unordered_map<Key,T>])
		AC_DEFINE(PROJECT[]_EXTEND_HASH_IN_STD_NS, true)	
		if test "${PROJECT[]_HAS_UNORDERED_MAP}" = true ; then
			AC_DEFINE(PROJECT[]_HAS_UNORDERED_MAP)
		else
			AC_MSG_RESULT(not found!)
		fi
	fi
	if test "${PROJECT[]_HAS_UNORDERED_MAP}" != true ; then
		AC_MSG_CHECKING(...trying hash_map)
		AC_MSG_CHECKING(for prefix of include paths for hash_map stl extension)
		for TMP_EXT_INCLUDE_PREFIX in "" "ext/"; do
			AC_TRY_COMPILE(
			[
			#include <${TMP_EXT_INCLUDE_PREFIX}hash_map>
			],
			[
			],
			EXT_INCLUDE_PREFIX_FOUND=true
			)
			if test "${EXT_INCLUDE_PREFIX_FOUND}" = "true" ; then
				break;
			fi;
		done
		if test "${EXT_INCLUDE_PREFIX_FOUND}" = "true" ; then
			AC_MSG_RESULT([found ("${TMP_EXT_INCLUDE_PREFIX}")])
			if test "${TMP_EXT_INCLUDE_PREFIX}" = "ext/" ; then
				AC_DEFINE(PROJECT[]_EXT_INCLUDE_PREFIX)
			fi

			dnl
			dnl now find out the correct namespace to use
			dnl
			AC_MSG_CHECKING(for namespace of hash_map)
			for TMP_EXT_NAMESPACE in "std" "stdext" "__gnu_cxx" "" ; do
				AC_TRY_COMPILE(
				[
				#include <${TMP_EXT_INCLUDE_PREFIX}hash_map>
				],
				[
				${TMP_EXT_NAMESPACE}::hash_map<int, int> a;
				],
				EXT_NAMESPACE_FOUND=true
				)
				if test "${EXT_NAMESPACE_FOUND}" = "true" ; then
					break;
				fi
			done
			if test "${EXT_NAMESPACE_FOUND}" = "true" ; then
				AC_MSG_RESULT([found ("${TMP_EXT_NAMESPACE}")])
				MAP_NAME="${TMP_EXT_NAMESPACE}::hash_map<Key,T>"
				AC_DEFINE_UNQUOTED(PROJECT[]_MAP_NAME, ${MAP_NAME})
				AC_DEFINE(PROJECT[]_HAS_HASH_MAP)
				PROJECT[]_MAP_NAMESPACE=${TMP_EXT_NAMESPACE}
				AC_DEFINE_UNQUOTED(PROJECT[]_MAP_NAMESPACE, ${PROJECT[]_MAP_NAMESPACE})
				HAS_HASH_MAP=true
			else
				AC_MSG_RESULT(not found! Using std::map instead!)
				AC_DEFINE(PROJECT[]_MAP_NAMESPACE, std)
				AC_DEFINE(PROJECT[]_MAP_NAME, [std::map<Key,T>])	
			fi
		else
			AC_MSG_RESULT(not found! Using std::map instead!)
			AC_DEFINE(PROJECT[]_MAP_NAMESPACE, std)
			AC_DEFINE(PROJECT[]_MAP_NAME, [std::map<Key,T>])	
		fi
	fi
	AC_SUBST(PROJECT[]_HAS_HASH_MAP)

	if test "${HAS_HASH_MAP}" = true ; then
		dnl
		dnl Now try to find out whether there is already a hash function for PROJECT[]::LongSize
		dnl
		AC_MSG_CHECKING(whether LongSize hash is required)
		AC_TRY_COMPILE(
		[
		#include <${TMP_EXT_INCLUDE_PREFIX}hash_map>

		namespace ${TMP_EXT_NAMESPACE} {
		template<>
		struct hash<${BALL_ULONG64_TYPE}>
		{
			size_t operator()(${BALL_ULONG64_TYPE} x) const { return (size_t)x; }
		};
		}
		],
		[
		],
		NEEDS_LONGSIZE_HASH=true
		)
		if test "${NEEDS_LONGSIZE_HASH}" = "true" ; then
			AC_MSG_RESULT(yes)
			AC_DEFINE(PROJECT[]_NEEDS_LONGSIZE_HASH)
		else
			AC_MSG_RESULT(no)
		fi
	fi
])

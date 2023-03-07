/**
 * author: Brando
 * date: 8/24/2022
 */

#ifndef ACCESS_SPECIFIERS
#define ACCESS_SPECIFIERS

#ifdef ASSERT_PUBLIC_MEMBER_ACCESS

#define PUBLIC public
#define PROTECTED public
#define PRIVATE public

#else // ASSERT_PUBLIC_MEMBER_ACCESS

#define PUBLIC public
#define PROTECTED protected
#define PRIVATE private

#endif // ASSERT_PUBLIC_MEMBER_ACCESS

#endif // Accessor Specifier

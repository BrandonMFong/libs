/**
 * author: Brando
 * date: 8/24/2022
 */

#ifndef ACCESS_HPP
#define ACCESS_HPP

#ifdef ASSERT_PUBLIC_MEMBER_ACCESS

#define PUBLIC public
#define PROTECTED public
#define PRIVATE public
#define public public
#define protected public
#define private public

#else // ASSERT_PUBLIC_MEMBER_ACCESS

#define PUBLIC public
#define PROTECTED protected
#define PRIVATE private
#define public public
#define protected protected
#define private private

#endif // ASSERT_PUBLIC_MEMBER_ACCESS

#endif // ACCESS_HPP

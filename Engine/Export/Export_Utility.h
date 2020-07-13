/*!
 * \file Export_Utility.h
 * \date 2015/04/03 1:44
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "LightMgr.h"
#include "NaviMgr.h"
#include "ResourceMgr.h"
#include "ShaderMgr.h"
#include "CollisionMgr.h"
#include "InfoSubject.h"

BEGIN(Engine)

inline CManagement* Get_Management(void);
inline CLightMgr* Get_LightMgr(void);
inline CNaviMgr* Get_NaviMgr(void);
inline CResourceMgr* Get_ResourceMgr(void);
inline CShaderMgr* Get_ShaderMgr(void);
inline CCollisionMgr* Get_CollisionMgr(void);
inline CInfoSubject* Get_InfoSubject(void);

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__
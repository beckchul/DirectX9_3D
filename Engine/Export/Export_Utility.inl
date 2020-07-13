
CManagement* Get_Management(void)
{
	return CManagement::GetInstance();
}

CLightMgr* Get_LightMgr(void)
{
	return CLightMgr::GetInstance();
}

CNaviMgr* Get_NaviMgr(void)
{
	return CNaviMgr::GetInstance();
}

CResourceMgr* Get_ResourceMgr(void)
{
	return CResourceMgr::GetInstance();
}

CShaderMgr* Get_ShaderMgr(void)
{
	return CShaderMgr::GetInstance();
}

CCollisionMgr* Get_CollisionMgr(void)
{
	return CCollisionMgr::GetInstance();
}

CInfoSubject* Get_InfoSubject(void)
{
	return CInfoSubject::GetInstance();
}
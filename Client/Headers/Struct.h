#ifndef _STRUCT_H__
#define _STRUCT_H__

struct Attribute
{
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _velocity;
	D3DXVECTOR3 _acceleration;
	float _lifeTime;
	float _age;
	D3DXCOLOR _color;
	D3DXCOLOR _colorFade;
	bool _isAlive;
};

//position: 월드 내의 파티클 위치
//velocity : 파티클의 속도, 보통 초당 이동 단위
//acceleration : 파티클의 가속, 보통 초당 이동 단위
//lifeTime : 파티클의 소멸까지 유지 시간
//age : 파티클의 현재 나이
//color : 파티클의 컬러
//clolorFade : 파티클의 컬러가 시간에 흐름에 따라 퇴색
//isAlive : 파티클 생존 시 TRUE, 소멸 시 FALSE

#endif	// _STRUCT_H__




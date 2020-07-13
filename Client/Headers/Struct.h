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

//position: ���� ���� ��ƼŬ ��ġ
//velocity : ��ƼŬ�� �ӵ�, ���� �ʴ� �̵� ����
//acceleration : ��ƼŬ�� ����, ���� �ʴ� �̵� ����
//lifeTime : ��ƼŬ�� �Ҹ���� ���� �ð�
//age : ��ƼŬ�� ���� ����
//color : ��ƼŬ�� �÷�
//clolorFade : ��ƼŬ�� �÷��� �ð��� �帧�� ���� ���
//isAlive : ��ƼŬ ���� �� TRUE, �Ҹ� �� FALSE

#endif	// _STRUCT_H__




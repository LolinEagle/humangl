#pragma once

#include <header.hpp>

using namespace std;

class VeCamera{
	private:
		vem::mat4	_projectionMatrix{1.f};
		vem::mat4	_viewMatrix{1.f};
		vem::mat4	_inverseViewMatrix{1.f};
	public:
		VeCamera(void);
		~VeCamera();

		void			setPerspectiveProjection(float fovy, float aspect, float near, float far);
		void			setViewYXZ(vem::vec3 position, vem::vec3 rotation);
		const vem::mat4	&getProjection(void) const;
		const vem::mat4	&getView(void) const;
		const vem::mat4	&getInverseView(void) const;
		const vem::vec3	getPosition(void) const;
};

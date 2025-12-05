#pragma once
#include"../model.h"
#include<glm/glm.hpp>
namespace myarcane {
	namespace graphics
	{
		class Renderable3D {//三维可渲染对象封装容器
		public:
			Renderable3D(glm::vec3 position,glm::vec3 scale,glm::vec3 rotationAxis,float radianRation, Model* model,bool shouldOutline =false,bool transparent=false);
			
			~Renderable3D();
			void draw(Shader& shader)const;
			inline glm::vec3& getPosition() { return m_Position; }
			inline glm::vec3 getScale() { return m_Scale; }//返回缩放比例
			inline glm::vec3 getRotationAxis() { return m_RotationAxis; }//返回旋转轴
			inline float getRadianRotation() { return m_RadianRotation; }//返回旋转弧度比例
			inline bool getShouldOutline() { return m_ShouldOutline; }//是否需要描边
			inline bool getTransparent() { return m_Transparent; }//是否为透明对象

			inline void setPosition(glm::vec3& position) { m_Position = position; }
			inline void setScale(glm::vec3& scale) { m_Scale = scale; }
			inline void setRotationAxis(glm::vec3& rotationAxis) { m_RotationAxis = rotationAxis; }
			inline void setRadianRotation(float radianRotation) { m_RadianRotation = radianRotation; }
			inline void setShouldOutline(bool shouldOutline) { m_ShouldOutline = shouldOutline; }//设置是否需要描边
			inline void setTransparent(bool transparent) { m_Transparent = transparent; }//设置是否为透明对象
		private:
			//model矩阵相关属性
			glm::vec3 m_Position;//位置
			glm::vec3 m_Scale;//缩放比例
			glm::vec3 m_RotationAxis;//旋转轴
			float m_RadianRotation;//旋转弧度比例,就是旋转角度乘以π/180得到的值
			bool m_ShouldOutline;//是否需要描边
			bool m_Transparent;//是否为透明对象
			Model* m_Model;//模型指针
		};
	}
}
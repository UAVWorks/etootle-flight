//     Copyright (c) 2013 js200300953@qq.com,lxyppc@163.com All rights reserved.
//         ========Բ�㲩ʿ΢��������������׳�������========
// 
// Բ�㲩ʿ΢��������������׳��������λ��������λ��Bootloader��
//     ��λ��App�������ǵ�Դ���룬�����ܳơ����򡱡�
// ������js200300953,lxyppc��д��
// �����Ϊʹ�����ṩ�ο���js200300953,lxyppc���Գ����ṩ�κ���ʾ�򰵺��ĵ�����
//     ������ʹ�øó����г��ֵ����������ʧ����
//     Ҳ������ʹ�øó��������ĵ��������⸺��
// ʹ���߿�����ѧϰΪĿ���޸ĺ�ʹ�øó�����������ҵ��Ŀ��ʹ�øó���
// �޸ĸó���ʱ�����뱣��ԭ��Ȩ���������Ҳ����޸�ԭ��Ȩ������
// 
// �������ϼ���
//     http://blog.sina.com.cn/js200300953
//     http://hi.baidu.com/lxyppc
//     http://www.etootle.com/
//     http://www.amobbs.com/thread-5504090-1-1.html
//     Բ�㲩ʿ΢�����������QQȺ��276721324

package com.lxyppc.flycontrol;

public class Quaternion {
	private static final float ZERO = 1e-4f;
	Quaternion(){
		loadIdentity();
	}
	    Quaternion(float w,float x,float y,float z){
	    	m_value[0] = w;
	        m_value[1] = x;
	        m_value[2] = y;
	        m_value[3] = z;
	    }
	    Quaternion(float yaw,float pitch,float roll){
	    	fromAngle(yaw,pitch,roll);
	    }
	    Quaternion(float[] val){
	    	assign(val);
	    }
	    float[] toMatrix(){
	    	float[] mat = new float[16];
	        float x2  = m_value[1] * 2;
	        float y2  = m_value[2] * 2;
	        float z2  = m_value[3] * 2;
	        float wx2 = m_value[0] * x2;
	        float wy2 = m_value[0] * y2;
	        float wz2 = m_value[0] * z2;
	        float xx2 = m_value[1] * x2;
	        float yy2 = m_value[2] * y2;
	        float zz2 = m_value[3] * z2;
	        float xy2 = m_value[1] * y2;
	        float yz2 = m_value[2] * z2;
	        float xz2 = m_value[3] * x2;
	        //
	        mat[0] = 1 - yy2 - zz2; mat[1] = xy2 - wz2;     mat[2] = xz2 + wy2;      mat[3] = 0;
	        mat[4] = xy2 + wz2;     mat[5] = 1 - xx2 - zz2; mat[6] = yz2 - wx2;      mat[7] = 0;
	        mat[8] = xz2 - wy2;     mat[9] = yz2 + wx2;     mat[10] = 1 - xx2 - yy2; mat[11] = 0;
	        mat[12] = 0;            mat[13] = 0;            mat[14] = 0;             mat[15] = 1;
	        return mat;
	    }
	    void nomalize(){
	        //
	        // ����ƽ���͡�
	        float norm = 0.0f;
	        for(int i=0;i<4;i++)
	            norm += m_value[i] * m_value[i];
	        //
	        // �ж��ݲ
	        norm = Math.abs(norm);
	        if(norm < ZERO)
	        {
	            loadIdentity();
	            return;
	        }
	        //
	        // ��λ����
	        norm = 1 / (float)Math.sqrt(norm);
	        for(int i=0;i<4;i++)
	            m_value[i] *= norm;
	    }
	    void loadIdentity(){
	    	m_value[0] = 1;
	        m_value[1] = m_value[2] = m_value[3] = 0;
	    }
	    void fromVectorRotation(float[] vector){
	        /*
	         * ȡ�����ĳ��ȡ� */
	        float norm = Math.abs(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
	        if(norm < ZERO)
	        {
	            loadIdentity();
	            return;
	        }
	        norm = (float) Math.sqrt(norm);
	        //
	        float cos_h = (float) Math.cos(norm/2);
	        m_value[0] = cos_h;
	        norm = (float) Math.sqrt(1-cos_h*cos_h) / norm; // ��sqrt��sin�졣
	        m_value[1] = vector[0] * norm;
	        m_value[2] = vector[1] * norm;
	        m_value[3] = vector[2] * norm;
	    }
	    void fromAngle(float yaw,float pitch,float roll){
	        float sin_yaw2 =  (float)Math.sin(yaw/2);
	        float cos_yaw2 = (float)Math.cos(yaw/2);
	        float sin_pitch2 = (float)Math.sin(pitch/2);
	        float cos_pitch2 = (float)Math.cos(pitch/2);
	        float sin_roll2 = (float)Math.sin(roll/2);
	        float cos_roll2 = (float)Math.cos(roll/2);
	        //
	        m_value[0] = cos_roll2*cos_pitch2*cos_yaw2 + sin_roll2*sin_pitch2*sin_yaw2;
	        m_value[1] = sin_roll2*cos_pitch2*cos_yaw2 - cos_roll2*sin_pitch2*sin_yaw2;
	        m_value[2] = cos_roll2*sin_pitch2*cos_yaw2 + sin_roll2*cos_pitch2*sin_yaw2;
	        m_value[3] = cos_roll2*cos_pitch2*sin_yaw2 - sin_roll2*sin_pitch2*cos_yaw2;
	    }
	    void multiply(Quaternion right){
	        float[] tmp = new float[4];
	        tmp[0] = m_value[0] * right.m_value[0] - m_value[1] * right.m_value[1] - m_value[2] * right.m_value[2] - m_value[3] * right.m_value[3];
	        tmp[1] = m_value[1] * right.m_value[0] + m_value[0] * right.m_value[1] + m_value[2] * right.m_value[3] - m_value[3] * right.m_value[2];
	        tmp[2] = m_value[2] * right.m_value[0] + m_value[0] * right.m_value[2] + m_value[3] * right.m_value[1] - m_value[1] * right.m_value[3];
	        tmp[3] = m_value[3] * right.m_value[0] + m_value[0] * right.m_value[3] + m_value[1] * right.m_value[2] - m_value[2] * right.m_value[1];
	        //
	        for(int i=0;i<4;i++)
	            m_value[i] = tmp[i];
	    }
	    void inversion(){
	    	for(int i=1;i<4;i++)
	            m_value[i] = - m_value[i];
	    }
	    void assign(float w,float x,float y,float z){
	    	m_value[0] = w;
	        m_value[1] = x;
	        m_value[2] = y;
	        m_value[3] = z;
	    }
	    void assign(float[] val){
	    	System.arraycopy(val, 0, m_value, 0, val.length);
	    }
	    float w() {return m_value[0];}
	    float x() {return m_value[1];}
	    float y() {return m_value[2];}
	    float z() {return m_value[3];}
	    float[]  value() {return m_value;}
	private  float[] m_value = new float[4];
}

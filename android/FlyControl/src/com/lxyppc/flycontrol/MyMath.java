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

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import android.util.Log;

public class MyMath {
	public static int crc16(int crc, byte[] data, int len){
		final int[] crc_tab = new int[]
			    {
			        0x0000 , 0x1021 , 0x2042 , 0x3063 , 0x4084 , 0x50A5 , 0x60C6 , 0x70E7 ,
			        0x8108 , 0x9129 , 0xA14A , 0xB16B , 0xC18C , 0xD1AD , 0xE1CE , 0xF1EF
			    };
		int h_crc;
	    for(int i=0;i<len;i++){
	    	byte b = data[i];
	    	h_crc = (crc>>12) & 0xf;
	    	crc <<= 4;
	    	crc ^= crc_tab[ (h_crc ^ (b>>4)) &0x0f ];
	    	crc &= 0xffff;
	    	h_crc = (crc>>12) & 0xf;
	    	crc <<= 4;
	    	crc ^= crc_tab[ h_crc ^ (b&0x0f) ];
	    	crc &= 0xffff;
	    }
	    return crc;
	}
	public static int crc16(int crc, byte[] data){
		return crc16(crc,data,data.length);
	}
	
	public static float[] toFloat(byte[] data, int offset, int count){
		int l = (data.length - offset) / 4;
		if(count>l)count = l;
		float[] r = new float[count];
		ByteBuffer byteBuf = ByteBuffer.wrap(data, offset, data.length - offset);
		byteBuf = byteBuf.order(null);
		for(int i=0;i<count;i++){
			r[i] = byteBuf.getFloat(i*4);
		}
		//DataInputStream ds = new DataInputStream(new ByteArrayInputStream(data, offset, data.length-offset));
		//for(int i=0;i<count;i++){
	    //	try {
				//r[i] = ds.readFloat();
		//	} catch (IOException e) {
		//		break;
		//	}
	    //}
		return r;
	}
	
	public static byte[] toByte(byte[] data, int offset){
		return toByte(data, offset, data.length);
	}
	
	public static byte[] toByte(byte[] data, int offset, int length){
		if(length > data.length)length = data.length;
		byte[] r = new byte[length - offset];
		System.arraycopy(data, offset, r, 0, r.length);
		return r;
	}
	
	public static byte[] toByte(float[] data){
		byte byteArray[] = new byte[data.length*4];  
		ByteBuffer byteBuf = ByteBuffer.wrap(byteArray); 
		byteBuf = byteBuf.order(null); 
		FloatBuffer floatBuf = byteBuf.asFloatBuffer();  
		floatBuf.put (data);
		return byteArray; 
	}
	
	public static byte[] toByte(short[] data){
		byte byteArray[] = new byte[data.length*2];  
		ByteBuffer byteBuf = ByteBuffer.wrap(byteArray); 
		byteBuf = byteBuf.order(null); 
		ShortBuffer shortBuf = byteBuf.asShortBuffer();
		shortBuf.put (data);
		return byteArray; 
	}
	
	public static void showBytes(byte[] data, String desc){
		String t = "";
		for(byte b:data){
			t += String.format("%x,", b<0?b+256:b);
		}
		Log.i("FLYC", desc + ": " + t);
	}
	
	public static void showBytes(float[] data, String desc){
		String t = "";
		for(float b:data){
			t += String.format("%f,", b);
		}
		Log.i("FLYC", desc + ": " + t);
	}
}

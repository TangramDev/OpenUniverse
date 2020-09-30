#pragma once

class CWormhole : public CSession
{
public:
	CWormhole();
	virtual ~CWormhole();

	void InsertString(CString key, CString value);
	void InsertLong(CString key, long value);
	void Insertint64(CString key, __int64 value);
	void InsertFloat(CString key, float value);
	CString GetString(CString key);
	long GetLong(CString key);
	__int64 Getint64(CString key);
	float GetFloat(CString key);
	void SendMessage();
	void addEventListener(CString ListenerName);
	void removeEventListener(CString ListenerName);

	IPCSession* m_pSession;
};


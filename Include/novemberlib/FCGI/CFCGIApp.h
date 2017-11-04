#ifndef CFCGIAPP_H
#define CFCGIAPP_H

class CFCGIApp
{
public:
	void run();

	CFCGIApp();
	virtual ~CFCGIApp();
protected:
	virtual void init();
	void requestHandler();
<<<<<<< HEAD
	void watchHandler();
=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
	int listenSocket;
};

#endif // CFCGIAPP_H

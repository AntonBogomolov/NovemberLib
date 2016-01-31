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
	int listenSocket;
};

#endif // CFCGIAPP_H

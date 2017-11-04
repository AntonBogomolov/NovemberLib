#ifndef CBAN_H
#define CBAN_H

class CBan
{
	public:
		CBan(const int id, const long roTime, const long banCreationTime, const int userId, bool isAccessClosed);
		CBan(const int id);

		int getId() const;
		int getUserId() const;
		long getBanCreadtionTime() const;
		long getROTime() const;
		bool getIsAccessClosed() const;

		~CBan();
	protected:
	private:
		int id;
		int userId;
		long roTime;
		long banCreationTime;
		bool isAccessClosed;

		void readFromDB();
};

#endif // CBAN_H

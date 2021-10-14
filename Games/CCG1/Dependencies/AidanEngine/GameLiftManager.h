#pragma once
#include <aws/gamelift/GameLiftClient.h>

class GameLiftManager
{
private:
	int m_Port; // set to 0 in constructor
	std::string m_AliasId;
	
	std::string m_ServerIpAddress; //c_str() for method
	std::string m_GameSessionId; 
	std::string m_PlayerSessionId;

	std::shared_ptr<Aws::GameLift::GameLiftClient> m_GLClient; //game lift client object
public:
	GameLiftManager(const std::string& alias);

	void SetUpAwsClient(const std::string& region);



	std::shared_ptr<Aws::GameLift::GameLiftClient> GetAwsClient(){ return m_GLClient;	}
	const std::string& GetAliasId() const{	return m_AliasId; }
};
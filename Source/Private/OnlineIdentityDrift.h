// Copyright 2016-2017 Directive Games Limited - All Rights Reserved.

#pragma once

#include "OnlineSubsystemDriftTypes.h"
#include "OnlineIdentityInterface.h"
#include "DriftAPI.h"

/**
 * Info associated with an user account generated by this online service
 */
class FUserOnlineAccountDrift : public FUserOnlineAccount
{
public:

    // FOnlineUser

    virtual TSharedRef<const FUniqueNetId> GetUserId() const override { return UserIdPtr; }
    virtual FString GetRealName() const override { return TEXT("DummyRealName"); }
    virtual FString GetDisplayName(const FString& Platform = FString()) const override  { return Name; }
    virtual bool GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const override;
    virtual bool SetUserAttribute(const FString& AttrName, const FString& AttrValue) override;

    // FUserOnlineAccount

    virtual FString GetAccessToken() const override { return TEXT("DummyAuthTicket"); }
    virtual bool GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const override;

    // FUserOnlineAccountDrift

    FUserOnlineAccountDrift(const TSharedRef<FUniqueNetIdDrift>& InUserId, const FString& InName)
    : UserIdPtr(InUserId)
    , Name(InName)
    { }

    virtual ~FUserOnlineAccountDrift()
    {
    }

    TSharedRef<const FUniqueNetId> UserIdPtr;

    FString Name;

    TMap<FString, FString> AdditionalAuthData;
    TMap<FString, FString> UserAttributes;
};

/**
 * Drift service implementation of the online identity interface
 */
class FOnlineIdentityDrift : public IOnlineIdentity
{
public:

    // IOnlineIdentity

    bool Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials) override;
    bool Logout(int32 LocalUserNum) override;
    bool AutoLogin(int32 LocalUserNum) override;
    TSharedPtr<FUserOnlineAccount> GetUserAccount(const FUniqueNetId& UserId) const override;
    TArray<TSharedPtr<FUserOnlineAccount> > GetAllUserAccounts() const override;
    TSharedPtr<const FUniqueNetId> GetUniquePlayerId(int32 LocalUserNum) const override;
    TSharedPtr<const FUniqueNetId> CreateUniquePlayerId(uint8* Bytes, int32 Size) override;
    TSharedPtr<const FUniqueNetId> CreateUniquePlayerId(const FString& Str) override;
    ELoginStatus::Type GetLoginStatus(int32 LocalUserNum) const override;
    ELoginStatus::Type GetLoginStatus(const FUniqueNetId& UserId) const override;
    FString GetPlayerNickname(int32 LocalUserNum) const override;
    FString GetPlayerNickname(const FUniqueNetId& UserId) const override;
    FString GetAuthToken(int32 LocalUserNum) const override;
	void RevokeAuthToken(const FUniqueNetId& LocalUserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate) override;
	void GetUserPrivilege(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate) override;
    FPlatformUserId GetPlatformUserIdFromUniqueNetId(const FUniqueNetId& UniqueNetId) const override;
    FString GetAuthType() const override;

    // FOnlineIdentityDrift

    /**
     * Constructor
     *
     * @param InSubsystem online subsystem being used
     */
    FOnlineIdentityDrift(class FOnlineSubsystemDrift* InSubsystem);

    /**
     * Destructor
     */
    virtual ~FOnlineIdentityDrift();

private:

    /**
     * Should use the initialization constructor instead
     */
    FOnlineIdentityDrift();

    bool LoginDedicatedServer();

    void OnAuthenticated(bool success, const FPlayerAuthenticatedInfo& info);
    void OnServerRegistered(bool success);

    /** Ids mapped to locally registered users */
    TMap<int32, TSharedPtr<const FUniqueNetId>> UserIds;

    /** Ids mapped to locally registered users */
    TMap<FUniqueNetIdDrift, TSharedRef<FUserOnlineAccountDrift>> UserAccounts;

    FOnlineSubsystemDrift* DriftSubsystem;

    FDelegateHandle onAuthenticatedHandle;
    FDelegateHandle onServerRegisteredHandle;
};

typedef TSharedPtr<FOnlineIdentityDrift, ESPMode::ThreadSafe> FOnlineIdentityNullPtr;

// Copyright © 2026 Nola Development Incubator. All Rights Reserved.

#include "TokenManager.h"
#include "JsonObjectConverter.h"
#include "Http.h"
#include "Misc/Base64.h"
#include "Misc/DateTime.h"

FTokenManager::FTokenManager()
	: KeyRotationInterval(24.0f) // 24 hours default
{
}

FTokenManager::~FTokenManager()
{
}

bool FTokenManager::Initialize(const FString& InJwksURL)
{
	if (InJwksURL.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("TokenManager: JWKS URL is empty"));
		return false;
	}

	JwksURL = InJwksURL;
	UE_LOG(LogTemp, Log, TEXT("TokenManager: Initialized with JWKS URL: %s"), *JwksURL);

	// Fetch initial JWKS
	return FetchJWKS();
}

bool FTokenManager::FetchJWKS()
{
	if (JwksURL.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("TokenManager: Cannot fetch JWKS, URL not set"));
		return false;
	}

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("GET"));
	Request->SetURL(JwksURL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// For now, we'll use a simplified synchronous approach
	// In production, this should be async
	UE_LOG(LogTemp, Log, TEXT("TokenManager: Fetching JWKS from %s"), *JwksURL);

	LastKeyFetch = FDateTime::UtcNow();

	// Note: This is a placeholder for actual HTTP request
	// In a real implementation, you would make an async HTTP call
	UE_LOG(LogTemp, Warning, TEXT("TokenManager: JWKS fetch requires async HTTP implementation"));

	return true;
}

bool FTokenManager::ValidateToken(const FString& TokenString, FJWTToken& OutToken)
{
	if (TokenString.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("TokenManager: Token string is empty"));
		return false;
	}

	TSharedPtr<FJsonObject> Header;
	TSharedPtr<FJsonObject> Payload;
	TArray<uint8> Signature;

	if (!ParseJWT(TokenString, Header, Payload, Signature))
	{
		UE_LOG(LogTemp, Error, TEXT("TokenManager: Failed to parse JWT"));
		return false;
	}

	// Extract header fields
	FString Algorithm = Header->GetStringField(TEXT("alg"));
	FString KeyId = Header->HasField(TEXT("kid")) ? Header->GetStringField(TEXT("kid")) : FString();

	if (Algorithm != TEXT("RS256"))
	{
		UE_LOG(LogTemp, Error, TEXT("TokenManager: Unsupported algorithm: %s"), *Algorithm);
		return false;
	}

	// Find the appropriate key
	const FJWK* Key = FindKey(KeyId);
	if (!Key)
	{
		UE_LOG(LogTemp, Error, TEXT("TokenManager: Key not found for kid: %s"), *KeyId);
		return false;
	}

	// Verify signature (simplified for now)
	// In production, implement full RS256 verification
	UE_LOG(LogTemp, Log, TEXT("TokenManager: Signature verification skipped (not yet implemented)"));

	// Extract claims
	OutToken.RawToken = TokenString;
	OutToken.Subject = Payload->HasField(TEXT("sub")) ? Payload->GetStringField(TEXT("sub")) : FString();
	OutToken.Issuer = Payload->HasField(TEXT("iss")) ? Payload->GetStringField(TEXT("iss")) : FString();
	
	if (Payload->HasField(TEXT("exp")))
	{
		OutToken.ExpirationTime = static_cast<int64>(Payload->GetNumberField(TEXT("exp")));
	}
	
	if (Payload->HasField(TEXT("iat")))
	{
		OutToken.IssuedAt = static_cast<int64>(Payload->GetNumberField(TEXT("iat")));
	}

	// Extract capabilities array
	if (Payload->HasField(TEXT("capabilities")))
	{
		const TArray<TSharedPtr<FJsonValue>>* CapabilitiesArray;
		if (Payload->TryGetArrayField(TEXT("capabilities"), CapabilitiesArray))
		{
			for (const TSharedPtr<FJsonValue>& Value : *CapabilitiesArray)
			{
				OutToken.Capabilities.Add(Value->AsString());
			}
		}
	}

	// Extract policy version
	if (Payload->HasField(TEXT("policy_version")))
	{
		OutToken.PolicyVersion = Payload->GetStringField(TEXT("policy_version"));
	}

	OutToken.bIsValid = true;

	UE_LOG(LogTemp, Log, TEXT("TokenManager: Token validated - Subject: %s, Capabilities: %d"), 
		*OutToken.Subject, OutToken.Capabilities.Num());

	return true;
}

bool FTokenManager::IsTokenExpired(const FJWTToken& Token) const
{
	if (!Token.bIsValid || Token.ExpirationTime == 0)
	{
		return true;
	}

	int64 CurrentTime = FDateTime::UtcNow().ToUnixTimestamp();
	return CurrentTime >= Token.ExpirationTime;
}

TArray<FString> FTokenManager::GetCapabilities(const FJWTToken& Token) const
{
	return Token.Capabilities;
}

FString FTokenManager::GetPolicyVersion(const FJWTToken& Token) const
{
	return Token.PolicyVersion;
}

bool FTokenManager::ShouldRotateKeys() const
{
	FTimespan TimeSinceLastFetch = FDateTime::UtcNow() - LastKeyFetch;
	return TimeSinceLastFetch.GetTotalHours() >= KeyRotationInterval;
}

void FTokenManager::RefreshKeys()
{
	UE_LOG(LogTemp, Log, TEXT("TokenManager: Refreshing keys"));
	FetchJWKS();
}

bool FTokenManager::ParseJWT(const FString& TokenString, TSharedPtr<FJsonObject>& OutHeader, TSharedPtr<FJsonObject>& OutPayload, TArray<uint8>& OutSignature)
{
	// JWT format: header.payload.signature
	TArray<FString> Parts;
	TokenString.ParseIntoArray(Parts, TEXT("."));

	if (Parts.Num() != 3)
	{
		UE_LOG(LogTemp, Error, TEXT("TokenManager: Invalid JWT format, expected 3 parts, got %d"), Parts.Num());
		return false;
	}

	// Decode header
	TArray<uint8> HeaderBytes = DecodeBase64Url(Parts[0]);
	FString HeaderJson = FString::FromBlob(HeaderBytes.GetData(), HeaderBytes.Num());
	TSharedRef<TJsonReader<>> HeaderReader = TJsonReaderFactory<>::Create(HeaderJson);
	if (!FJsonSerializer::Deserialize(HeaderReader, OutHeader) || !OutHeader.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("TokenManager: Failed to parse JWT header"));
		return false;
	}

	// Decode payload
	TArray<uint8> PayloadBytes = DecodeBase64Url(Parts[1]);
	FString PayloadJson = FString::FromBlob(PayloadBytes.GetData(), PayloadBytes.Num());
	TSharedRef<TJsonReader<>> PayloadReader = TJsonReaderFactory<>::Create(PayloadJson);
	if (!FJsonSerializer::Deserialize(PayloadReader, OutPayload) || !OutPayload.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("TokenManager: Failed to parse JWT payload"));
		return false;
	}

	// Decode signature
	OutSignature = DecodeBase64Url(Parts[2]);

	return true;
}

bool FTokenManager::VerifyRS256Signature(const FString& Data, const TArray<uint8>& Signature, const FJWK& Key)
{
	// RS256 signature verification
	// This is a placeholder - full implementation requires crypto library
	UE_LOG(LogTemp, Warning, TEXT("TokenManager: RS256 signature verification not yet implemented"));
	return true;
}

const FJWK* FTokenManager::FindKey(const FString& KeyId) const
{
	for (const FJWK& Key : Keys)
	{
		if (Key.KeyId == KeyId)
		{
			return &Key;
		}
	}
	return nullptr;
}

bool FTokenManager::ParseJWKS(const FString& JwksJson)
{
	TSharedPtr<FJsonObject> JwksObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JwksJson);

	if (!FJsonSerializer::Deserialize(Reader, JwksObject) || !JwksObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("TokenManager: Failed to parse JWKS JSON"));
		return false;
	}

	const TArray<TSharedPtr<FJsonValue>>* KeysArray;
	if (!JwksObject->TryGetArrayField(TEXT("keys"), KeysArray))
	{
		UE_LOG(LogTemp, Error, TEXT("TokenManager: JWKS missing 'keys' array"));
		return false;
	}

	Keys.Empty();

	for (const TSharedPtr<FJsonValue>& KeyValue : *KeysArray)
	{
		TSharedPtr<FJsonObject> KeyObject = KeyValue->AsObject();
		if (!KeyObject.IsValid())
		{
			continue;
		}

		FJWK NewKey;
		NewKey.KeyId = KeyObject->GetStringField(TEXT("kid"));
		NewKey.KeyType = KeyObject->GetStringField(TEXT("kty"));
		NewKey.Algorithm = KeyObject->GetStringField(TEXT("alg"));
		NewKey.Use = KeyObject->HasField(TEXT("use")) ? KeyObject->GetStringField(TEXT("use")) : FString();
		NewKey.Modulus = KeyObject->GetStringField(TEXT("n"));
		NewKey.Exponent = KeyObject->GetStringField(TEXT("e"));

		// Decode modulus and exponent
		NewKey.ModulusBytes = DecodeBase64Url(NewKey.Modulus);
		NewKey.ExponentBytes = DecodeBase64Url(NewKey.Exponent);

		Keys.Add(NewKey);
		UE_LOG(LogTemp, Log, TEXT("TokenManager: Loaded key - ID: %s, Type: %s, Algorithm: %s"), 
			*NewKey.KeyId, *NewKey.KeyType, *NewKey.Algorithm);
	}

	return Keys.Num() > 0;
}

TArray<uint8> FTokenManager::DecodeBase64Url(const FString& Base64String) const
{
	// Base64URL uses '-' instead of '+' and '_' instead of '/'
	// Also, padding '=' characters may be omitted
	FString Base64 = Base64String;
	Base64 = Base64.Replace(TEXT("-"), TEXT("+"));
	Base64 = Base64.Replace(TEXT("_"), TEXT("/"));

	// Add padding if needed
	int32 Padding = (4 - (Base64.Len() % 4)) % 4;
	for (int32 i = 0; i < Padding; i++)
	{
		Base64.AppendChar('=');
	}

	TArray<uint8> DecodedData;
	FBase64::Decode(Base64, DecodedData);

	return DecodedData;
}

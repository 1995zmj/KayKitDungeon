#include "LoadingScreen.h"
#include "MoviePlayer.h"

#define LOCTEXT_NAMESPACE "FLoadingScreenModule"
#undef LOCTEXT_NAMESPACE

struct FKayLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FKayLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(nullptr, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		if (UObject* CachedResouceObject = GetResourceObject())
		{
			Collector.AddReferencedObject(CachedResouceObject);
		}
	}
};

class SKayLoadingScreen: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SKayLoadingScreen){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		static const FName LoadingScreenName(TEXT("/Game/UI/T_ActionRPG_TransparentLogo.T_ActionRPG_TransparentLogo"));

		LoadingScreenBrush = MakeShareable(new FKayLoadingScreenBrush(LoadingScreenName, FVector2D(1024,256)));

		FSlateBrush *BGBrush = new FSlateBrush();
		BGBrush->TintColor = FLinearColor(0.034f, 0.034f, 0.034f, 1.f);
	}
private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		bool Vis = GetMoviePlayer()->IsLoadingFinished();
		return Vis ? EVisibility::Collapsed : EVisibility::Visible;
	}

	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};

class FLoadingScreenModule : public ILoadingScreenModule
{
public:
	virtual void StartupModule() override
	{
		// 预计加载图片
		LoadObject<UObject>(nullptr, TEXT(""));

		if (IsMoviePlayerEnabled())
		{
			
		}
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}
	
	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) override
	{
		
	}
	
	virtual void StopInGameLoadingScreen() override
	{
		
	}

	virtual void CreateScreen()
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 3.f;
		LoadingScreen.WidgetLoadingScreen = SNew(SKayLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
};
    
IMPLEMENT_MODULE(FLoadingScreenModule, LoadingScreen)
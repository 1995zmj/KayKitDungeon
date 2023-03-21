#include "LoadingScreen.h"
#include "MoviePlayer.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Images/SThrobber.h"

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
		static const FName LoadingScreenName(TEXT("/Game/Dungeon/UI/T_ActionRPG_TransparentLogo.T_ActionRPG_TransparentLogo"));

		LoadingScreenBrush = MakeShareable(new FKayLoadingScreenBrush(LoadingScreenName, FVector2D(1024,256)));

		FSlateBrush *BGBrush = new FSlateBrush();
		BGBrush->TintColor = FLinearColor(0.034f, 0.034f, 0.034f, 1.f);

		ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.BorderImage(BGBrush)
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Bottom)
				.HAlign(HAlign_Right)
				.Padding(FMargin(10.f))
				[
					SNew(SThrobber)
					.Visibility(this, &SKayLoadingScreen::GetLoadIndicatorVisibility)
				]
			]
		];
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
		LoadObject<UObject>(nullptr, TEXT("/Game/Dungeon/UI/T_ActionRPG_TransparentLogo.T_ActionRPG_TransparentLogo"));

		if (IsMoviePlayerEnabled())
		{
			CreateScreen();
		}
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}
	
	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) override
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = !bPlayUntilStopped;
		LoadingScreen.bWaitForManualStop = bPlayUntilStopped;
		LoadingScreen.bAllowEngineTick = bPlayUntilStopped;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = SNew(SKayLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
	
	virtual void StopInGameLoadingScreen() override
	{
		GetMoviePlayer()->StopMovie();
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
    
IMPLEMENT_GAME_MODULE(FLoadingScreenModule, LoadingScreen)
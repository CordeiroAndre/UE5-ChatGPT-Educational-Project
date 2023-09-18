// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/SimplerBaseWidget.h"



void USimplerBaseWidget::SetBase(USimplerActivatableWidget* Base)
{
	bool HadToRemoveWidget = false;
	if(BasePanel->GetAllChildren().Num() > 0)
		if (UWidget* Child = BasePanel->GetAllChildren()[0])
		{
			if(USimplerActivatableWidget* SimplerWidget = Cast<USimplerActivatableWidget>(Child))
			{
				if(SimplerWidget->DeactivationAnim)
				{
					FWidgetAnimationDynamicEvent RemoveEvent;
					RemoveEvent.BindUFunction(this, FName("RemoveWidgetFromBase"));
					SimplerWidget->BindToAnimationFinished(SimplerWidget->DeactivationAnim, RemoveEvent);
					SimplerWidget->PlayAnimation(SimplerWidget->DeactivationAnim);
				}
				else
				{
					RemoveWidgetFromBase();
				}
			}
			HadToRemoveWidget = true;
		}

	if(!HadToRemoveWidget)
	{
		Base->PlayAnimation(Base->ActivationAnim);
	}
	_widgetToActivate = Base;
	BasePanel->AddChild(Base);

}

void USimplerBaseWidget::SetPrompt(USimplerActivatableWidget* Prompt)
{
}

void USimplerBaseWidget::RemoveWidgetFromBase()
{
	BasePanel->RemoveChildAt(0);
	BasePanel->AddChild(_widgetToActivate);
	_widgetToActivate->PlayAnimation(_widgetToActivate->ActivationAnim);

}

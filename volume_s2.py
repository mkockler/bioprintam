# volume_s2.py
from kivy.app import App
from kivy.uix.screenmanager import Screen
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.label import Label

class VolumeS2Screen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        layout = BoxLayout(orientation='vertical', spacing=10, padding=10)

        layout.add_widget(Label(text="Volume S2 (mL)"))

        for vol in ["0.0", "0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9"]:
            btn = Button(text=f"{vol} mL")
            btn.bind(on_press=self.select_volume)
            layout.add_widget(btn)

        back_btn = Button(text="Back")
        back_btn.bind(on_press=self.go_back)
        layout.add_widget(back_btn)

        self.add_widget(layout)

    def select_volume(self, instance):
        App.get_running_app().volume_s2 = instance.text  # Save selected volume globally
        self.manager.current = 'home'  # Return to home screen

    def go_back(self, instance):
        self.manager.current = 'home'
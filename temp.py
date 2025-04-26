from kivy.app import App
from kivy.uix.screenmanager import Screen
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.label import Label

class TemperatureScreen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        layout = BoxLayout(orientation='vertical', spacing=10, padding=10)

        temps = ["20", "37", "40", "45", "50", "55", "60", "65"]
        for temp in temps:
            btn = Button(text=f"{temp} °C")
            btn.bind(on_press=self.select_temperature)
            layout.add_widget(btn)

        back_btn = Button(text="Back")
        back_btn.bind(on_press=self.go_back)
        layout.add_widget(back_btn)

        self.add_widget(layout)

    def select_temperature(self, instance):
        App.get_running_app().temperature = instance.text  # Save to global App variable
        self.manager.current = 'home'  # Go back to home

    def go_back(self, instance):
        self.manager.current = 'home'

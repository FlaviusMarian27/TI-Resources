import { Tabs } from 'expo-router';

export default function RootLayout() {
  return (
    <Tabs>
      <Tabs.Screen
        name="(activitati)"
        options={{ title: 'Activități', headerShown: false }}
      />
      <Tabs.Screen name="despre" options={{ title: 'Despre' }} />
    </Tabs>
  );
}
import { Stack } from 'expo-router';

export default function ActivitatiLayout() {
  return (
    <Stack>
      <Stack.Screen name="index" options={{ title: 'Activități' }} />
      <Stack.Screen name="[id]" options={{ title: 'Detalii activitate' }} />
      <Stack.Screen name="adauga" options={{ title: 'Adaugă activitate' }} />
    </Stack>
  );
}
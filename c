-- Hook RemoteEvent untuk melihat argumen saat ada player lain memberi hadiah
local Remote = game:GetService("ReplicatedStorage"):WaitForChild("AcceptPetGift")

Remote.OnClientEvent:Connect(function(...)
    print("🎁 AcceptPetGift triggered with args:")
    print(...)
end)

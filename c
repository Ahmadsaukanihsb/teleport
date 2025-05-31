local Players = game:GetService("Players")
local ReplicatedStorage = game:GetService("ReplicatedStorage")
local TextChatService = game:GetService("TextChatService")

local localPlayer = Players.LocalPlayer
local GIFT_COOLDOWN = 3
local lastGiftCheck = 0

-- Improved function to accept friend gifts
local function acceptFriendGifts()
    local friendGiftEvent = ReplicatedStorage:FindFirstChild("FriendGiftEvent") or ReplicatedStorage:FindFirstChild("FriendGiftRemote")
    if not friendGiftEvent then return false end

    -- Try different parameter formats based on common game implementations
    local success, response = pcall(function()
        -- Try simple string first
        if friendGiftEvent:IsA("RemoteEvent") then
            return friendGiftEvent:FireServer("AcceptAllGifts")
        elseif friendGiftEvent:IsA("RemoteFunction") then
            return friendGiftEvent:InvokeServer("AcceptAllGifts")
        end
    end)
    
    if not success then
        success = pcall(function()
            -- Try with player ID
            if friendGiftEvent:IsA("RemoteEvent") then
                friendGiftEvent:FireServer({Action = "AcceptAll", PlayerId = localPlayer.UserId})
            end
        end)
    end
    
    return success
end

-- Improved function to accept pet gifts
local function acceptPetGifts()
    local petGiftService = ReplicatedStorage:FindFirstChild("PetGiftingService") or ReplicatedStorage:FindFirstChild("PetGiftRemote")
    if not petGiftService then return false end

    local success = pcall(function()
        if petGiftService:IsA("RemoteEvent") then
            petGiftService:FireServer("ClaimAllGifts")
        elseif petGiftService:IsA("RemoteFunction") then
            petGiftService:InvokeServer("ClaimAllGifts")
        end
        return true
    end)
    
    if not success then
        success = pcall(function()
            if petGiftService:IsA("RemoteEvent") then
                petGiftService:FireServer({
                    Action = "ClaimAll",
                    UserId = localPlayer.UserId
                })
            end
        end)
    end
    
    return success
end

-- Main function with better error handling
local function collectAllGifts()
    if os.time() - lastGiftCheck < GIFT_COOLDOWN then return end
    lastGiftCheck = os.time()

    print("\n[System] Checking for new gifts...")
    
    -- Accept friend gifts
    local friendSuccess = acceptFriendGifts()
    print(friendSuccess and "[Success] Friend gifts claimed" or "[Warning] No friend gifts available")
    
    -- Accept pet gifts
    local petSuccess = acceptPetGifts()
    print(petSuccess and "[Success] Pet gifts claimed" or "[Warning] No pet gifts available")

    -- Anti-AFK activity
    if math.random(1, 10) == 1 then
        pcall(function()
            TextChatService.TextChannels.RBXGeneral:SendAsync("Gift collector active - checking for new gifts")
        end)
    end
end

-- Auto-execute with better error handling
while task.wait(GIFT_COOLDOWN) do
    local success, err = pcall(collectAllGifts)
    if not success then
        warn("[Error] Gift collection failed:", err)
    end
end

print("Auto-gift collector running successfully!")
